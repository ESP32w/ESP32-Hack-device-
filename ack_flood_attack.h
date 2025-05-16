// ack_flood_attack.h
#ifndef ACK_FLOOD_ATTACK_H
#define ACK_FLOOD_ATTACK_H

#include "data_structures.h"
#include <WiFi.h> // For esp_wifi_80211_tx
#include <esp_wifi.h> // For WIFI_IF_STA

// Structure to hold ACK Flood attack settings
struct AckFloodAttackSettings {
    uint8_t target_bssid[6];
    int num_packets; // Number of ACK packets to send
    int packet_interval_ms; // Interval between packets in milliseconds
    int attack_duration_s; // Duration of the attack in seconds (0 for indefinite until stopped)
};

// External declaration of stats and global variables
extern AckFloodStats ackFloodDetailedStats;
extern volatile unsigned long totalPacketsSentGlobal;

// Function Prototypes
void start_ack_flood_attack(const AckFloodAttackSettings& settings);
void stop_ack_flood_attack(); // To stop an ongoing attack
void perform_sendAckFlood_custom(const AckFloodAttackSettings& settings); // Internal function

bool parseMacAddress(const char* ms, uint8_t* ma); // Declaration, will be in utilities
String macToString(const uint8_t* mac); // Declaration, will be in utilities

#endif // ACK_FLOOD_ATTACK_H

