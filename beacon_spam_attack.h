// beacon_spam_attack.h
#ifndef BEACON_SPAM_ATTACK_H
#define BEACON_SPAM_ATTACK_H

#include "data_structures.h"
#include <WiFi.h> // For esp_wifi_80211_tx, esp_wifi_get_channel
#include <esp_wifi.h> // For WIFI_IF_STA
#include <esp_timer.h> // For esp_timer_get_time()
#include <vector> // For std::vector
#include <string> // For std::string

// Structure to hold Beacon Spam attack settings
struct BeaconSpamAttackSettings {
    int num_ssids; // Number of fake SSIDs to generate
    int packet_interval_ms; // Interval between sending beacon packets for each SSID
    // std::vector<std::string> custom_ssid_list; // Optional: list of custom SSIDs to use
    // bool use_custom_ssids; // True if custom_ssid_list should be used
    uint8_t encryption_type; // 0 for Open, 1 for WPA_PSK (simplified)
    // int attack_duration_s; // Duration of the attack in seconds (0 for indefinite until stopped) - Beacon spam is usually continuous
};

// External declaration of stats and global variables
extern BeaconSpamStats beaconSpamDetailedStats;
extern volatile unsigned long totalPacketsSentGlobal;
// extern const uint16_t BEACON_INTERVAL_TU; // This might be dynamic now or part of settings

// Function Prototypes
void start_beacon_spam_attack(const BeaconSpamAttackSettings& settings);
void stop_beacon_spam_attack(); // To stop an ongoing attack
void perform_sendBeaconSpam_custom(const BeaconSpamAttackSettings& settings); // Internal function

// Utility functions (declarations, definitions might be in .cpp or utilities)
void generateRandomMac(uint8_t* ma);
String generateRandomSSID(int len = 8);
String macToString(const uint8_t* mac);

#endif // BEACON_SPAM_ATTACK_H

