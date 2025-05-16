// probe_flood_attack.h
#ifndef PROBE_FLOOD_ATTACK_H
#define PROBE_FLOOD_ATTACK_H

#include "data_structures.h"
#include <WiFi.h> // For esp_wifi_80211_tx
#include <esp_wifi.h> // For WIFI_IF_STA

// Structure to hold Probe Flood attack settings
struct ProbeFloodAttackSettings {
    int num_packets; // Number of probe request packets to send
    int packet_interval_ms; // Interval between packets in milliseconds
    int attack_duration_s; // Duration of the attack in seconds (0 for indefinite until stopped)
    // char target_ssid[33]; // Optional: Specific SSID to probe for, empty for broadcast probe
};

// External declaration of stats and global variables
extern ProbeFloodStats probeFloodDetailedStats;
extern volatile unsigned long totalPacketsSentGlobal;

// Function Prototypes
void start_probe_flood_attack(const ProbeFloodAttackSettings& settings);
void stop_probe_flood_attack(); // To stop an ongoing attack
void perform_sendProbeFlood_custom(const ProbeFloodAttackSettings& settings); // Internal function

// Utility functions (declarations, definitions might be in .cpp or utilities)
void generateRandomMac(uint8_t* ma);
String macToString(const uint8_t* mac);

#endif // PROBE_FLOOD_ATTACK_H

