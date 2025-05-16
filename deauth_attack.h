// deauth_attack.h
#ifndef DEAUTH_ATTACK_H
#define DEAUTH_ATTACK_H

#include "data_structures.h"
#include <WiFi.h> // For esp_wifi_80211_tx
#include <esp_wifi.h> // For WIFI_IF_STA

// Structure to hold Deauth attack settings
struct DeauthAttackSettings {
    uint8_t target_bssid[6];
    uint8_t target_client_mac[6]; // FF:FF:FF:FF:FF:FF for broadcast
    int num_packets; // Number of deauth packets to send
    int packet_interval_ms; // Interval between packets in milliseconds
    int attack_duration_s; // Duration of the attack in seconds (0 for indefinite until stopped)
    bool use_broadcast_client; // True if target_client_mac should be broadcast
};

// External declaration of deauthDetailedStats, will be defined in the main .ino file
extern DeauthStats deauthDetailedStats;
extern volatile unsigned long totalPacketsSentGlobal;
extern std::vector<APInfo> foundAPsList; // Used to auto-select BSSID if not provided

// Function Prototypes
void start_deauth_attack(const DeauthAttackSettings& settings);
void stop_deauth_attack(); // To stop an ongoing attack
void perform_sendDeauth_custom(const DeauthAttackSettings& settings); // Internal function, might be called by a task

// Utility functions (declarations, definitions might be in .cpp or utilities)
bool parseMacAddress(const char* ms, uint8_t* ma);
String macToString(const uint8_t* mac);

#endif // DEAUTH_ATTACK_H

