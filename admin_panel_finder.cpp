// admin_panel_finder.cpp
#include "admin_panel_finder.h"

// Common admin panel paths
const std::vector<String> default_admin_paths = {
    "/admin/", "/administrator/", "/admin.php", "/admin.html", "/login.php", "/admin_login.php",
    "/panel/", "/controlpanel/", "/cpanel/", "/admincp/", "/user/", "/login/", "/signin/",
    "/wp-admin/", "/wp-login.php", "/joomla/administrator/", "/drupal/user/login/",
    "/admin1/", "/admin2/", "/adminarea/", "/manage/", "/webadmin/", "/myadmin/", "/phpmyadmin/",
    "/admin/login.php", "/administrator/login.php", "/admin/index.php", "/administrator/index.php",
    "/admin.asp", "/admin.aspx", "/admin/login.asp", "/admin/login.aspx", "/admin_area/"
};

AdminPanelFinder::AdminPanelFinder() {
    // Constructor
}

String AdminPanelFinder::normalizeUrl(String url) {
    url.trim();
    if (!url.startsWith("http://") && !url.startsWith("https://")) {
        url = "http://" + url; // Default to http if no protocol
    }
    if (url.endsWith("/")) {
        url = url.substring(0, url.length() - 1);
    }
    return url;
}

// Updated checkPath to use WebExploitSettings
String AdminPanelFinder::checkPath(String baseUrl, String path, const WebExploitSettings& settings) {
    String fullUrl = baseUrl + (path.startsWith("/") ? path : "/" + path);
    String result = "";

    http.begin(fullUrl);
    http.setUserAgent(settings.currentUserAgent);
    if (!settings.currentReferer.isEmpty()) {
        http.addHeader("Referer", settings.currentReferer);
    }
    http.setFollowRedirects(true); 
    http.setTimeout(7000); 

    int httpCode = http.GET();

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            result = "Found (200 OK): " + fullUrl;
        } else if (httpCode == HTTP_CODE_UNAUTHORIZED) {
            result = "Potential (401 Unauthorized): " + fullUrl;
        } else if (httpCode == HTTP_CODE_FORBIDDEN) {
            result = "Potential (403 Forbidden): " + fullUrl;
        }
    } 
    http.end();
    return result;
}

// Updated beginTest to use WebExploitSettings
String AdminPanelFinder::beginTest(String baseUrl, const std::vector<String>& pathsToTry, const WebExploitSettings& settings) {
    String results = "Admin Panel Finder Started.\nBase URL: " + baseUrl + "\n";
    results += "User-Agent: " + settings.currentUserAgent + "\n";
    if (!settings.currentReferer.isEmpty()) results += "Referer: " + settings.currentReferer + "\n";
    results += "Request Delay: " + String(settings.requestDelayMs) + "ms (Random: " + (settings.useRandomDelay ? "Yes" : "No") + ")\n";

    String normalizedBaseUrl = normalizeUrl(baseUrl);
    results += "Normalized Base URL: " + normalizedBaseUrl + "\n";

    const std::vector<String>& current_paths = pathsToTry.empty() ? default_admin_paths : pathsToTry;

    if (normalizedBaseUrl.isEmpty()) {
        results += "Error: Base URL is empty.\n";
        return results;
    }

    int foundCount = 0;
    for (const String& path : current_paths) {
        String finding = checkPath(normalizedBaseUrl, path, settings);
        if (!finding.isEmpty()) {
            results += "  -> " + finding + "\n";
            foundCount++;
        }
        delay(settings.getEffectiveDelay()); 
    }

    if (foundCount == 0) {
        results += "No common admin panels found with the checked paths.\n";
    }
    results += "Admin Panel Finder Finished.\n";
    return results;
}

