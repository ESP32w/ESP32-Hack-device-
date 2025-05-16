// xss_tester.h
#ifndef XSS_TESTER_H
#define XSS_TESTER_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include "web_exploit_config.h" // Added for security settings

class XSSTester {
public:
    XSSTester();
    // Modified to accept WebExploitSettings
    String beginTest(String targetUrl, String targetParameter, const std::vector<String>& payloads, const WebExploitSettings& settings);
    String checkPageForXSS(const String& pageContent, const String& payload);

private:
    HTTPClient http;
    // Modified to accept WebExploitSettings
    String sendRequest(String url, String method, String data, const WebExploitSettings& settings);
    std::vector<String> extractForms(const String& htmlContent);
    std::vector<String> extractParamsFromForm(const String& formHtml);
    String constructUrlWithPayload(String baseUrl, String paramName, String payload);
    String constructPostDataWithPayload(const std::vector<String>& params, String targetParam, String payload);
};

#endif // XSS_TESTER_H

