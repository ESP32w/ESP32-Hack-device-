// sql_injection_scanner.h
#ifndef SQL_INJECTION_SCANNER_H
#define SQL_INJECTION_SCANNER_H

#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include "web_exploit_config.h" // Added for security settings

class SQLInjectionScanner {
public:
    SQLInjectionScanner();
    // Modified to accept WebExploitSettings
    String beginTest(String targetUrl, String targetParameter, const std::vector<String>& sqlPayloads, const WebExploitSettings& settings);

private:
    HTTPClient http;
    // Modified to accept WebExploitSettings
    String sendRequestWithPayload(String baseUrl, String paramName, String payload, bool isPost, const WebExploitSettings& settings);
    String checkResponseForSQLVulnerability(const String& responseContent, const String& payload);
    String constructUrlWithPayload(String baseUrl, String paramName, String payload);
    String constructPostDataWithPayload(String paramName, String payload);

    // Common SQL error patterns
    const std::vector<String> sql_error_indicators = {
        "SQL syntax", "mysql_fetch", "mysql_query", "pg_query", "ORA-01756", "ORA-00933", 
        "Microsoft OLE DB Provider for SQL Server", "Unclosed quotation mark after the character string",
        "You have an error in your SQL syntax", "Query failed", "supplied argument is not a valid MySQL result resource",
        "Syntax error", "Parameter q is not valid", "Invalid SQL statement"
    };
};

#endif // SQL_INJECTION_SCANNER_H

