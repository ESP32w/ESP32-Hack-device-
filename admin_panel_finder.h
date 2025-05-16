// admin_panel_finder.h
#ifndef ADMIN_PANEL_FINDER_H
#define ADMIN_PANEL_FINDER_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include "web_exploit_config.h" // Added for security settings

class AdminPanelFinder {
public:
    AdminPanelFinder();
    // Modified to accept WebExploitSettings
    String beginTest(String baseUrl, const std::vector<String>& pathsToTry, const WebExploitSettings& settings);

private:
    HTTPClient http;
    // Modified to accept WebExploitSettings
    String checkPath(String baseUrl, String path, const WebExploitSettings& settings);
    String normalizeUrl(String url);
};

#endif // ADMIN_PANEL_FINDER_H

