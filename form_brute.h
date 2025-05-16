// form_brute.h
#ifndef FORM_BRUTE_H
#define FORM_BRUTE_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include <FS.h> // For SD card access if used
#include "web_exploit_config.h" // Added for security settings

// Forward declaration for callback function type
class FormBrute;
typedef void (*FormBruteProgressCallback)(int percentage, const String& current_attempt, const String& status_message);

class FormBrute {
public:
    FormBrute();
    // Modified to accept WebExploitSettings
    String beginTest(String targetUrl, String loginUrl, String userParam, String passParam, String successIdentifier, const std::vector<String>& usernames, const std::vector<String>& passwords, const WebExploitSettings& settings, FormBruteProgressCallback progressCallback);
    String beginTestWithDictionary(String targetUrl, String loginUrl, String userParam, String passParam, String successIdentifier, String dictionaryPath, bool isUsernameList, const WebExploitSettings& settings, FormBruteProgressCallback progressCallback); // For SD card dictionaries

private:
    HTTPClient http;
    // Modified to accept WebExploitSettings
    String sendLoginAttempt(String loginUrl, String userParam, String username, String passParam, String password, const WebExploitSettings& settings);
    bool checkLoginSuccess(const String& responseContent, const String& successIdentifier);
    std::vector<String> loadDictionary(fs::FS &fs, const String& path); // Helper for SD card
    String constructPostData(String userParam, String username, String passParam, String password, const std::vector<String>& otherFormParams);
    std::vector<String> extractOtherFormParams(const String& formPageHtml, const std::vector<String>& excludeParams);

    // Callback for progress updates
    FormBruteProgressCallback _progressCallback;
};

#endif // FORM_BRUTE_H

