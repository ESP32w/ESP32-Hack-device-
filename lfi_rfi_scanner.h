// lfi_rfi_scanner.h
#ifndef LFI_RFI_SCANNER_H
#define LFI_RFI_SCANNER_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include "web_exploit_config.h" // Added for security settings

class LFIRFIScanner {
public:
    LFIRFIScanner();
    // Modified to accept WebExploitSettings
    String beginTest(String targetUrl, String targetParameter, const std::vector<String>& lfiPayloads, const std::vector<String>& rfiPayloads, const WebExploitSettings& settings);

private:
    HTTPClient http;
    // Modified to accept WebExploitSettings
    String sendRequestWithPayload(String baseUrl, String paramName, String payload, const WebExploitSettings& settings);
    String checkResponseForVulnerability(const String& responseContent, const String& payload, const String& vulnerabilityType);
    String constructUrlWithPayload(String baseUrl, String paramName, String payload);

    // Predefined content to look for in LFI successful responses
    const std::vector<String> lfi_success_indicators = {
        "root:x:0:0", // /etc/passwd
        "; for 16-bit app support" // win.ini
    };
};

#endif // LFI_RFI_SCANNER_H

