// data_structures.h
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <Arduino.h>
#include <vector>
#include <esp_wifi.h> // For wifi_promiscuous_pkt_t

// --- Attack State Management ---
enum AttackType {
    NONE,
    DEAUTH,
    BEACON_SPAM,
    PROBE_FLOOD,
    ACK_FLOOD,
    PACKET_INJECTION, // Not implemented
    PING_OF_DEATH     // Not implemented
};

// --- Detailed Statistics Variables ---
struct DeauthStats { 
    unsigned long packetsSent = 0; 
    String targetMac = "FF:FF:FF:FF:FF:FF"; 
    unsigned long errors = 0; 
    String targetBssid = ""; 
};

struct BeaconSpamStats { 
    unsigned long packetsSent = 0; 
    unsigned int ssidsGenerated = 0; 
    unsigned long errors = 0; 
    String lastGeneratedSSID = "N/A"; 
};

struct ProbeFloodStats { 
    unsigned long packetsSent = 0; 
    unsigned long errors = 0; 
    String targetSSID = "Wildcard"; 
};

struct AckFloodStats { 
    unsigned long packetsSent = 0; 
    unsigned long errors = 0; 
    String targetMac = "FF:FF:FF:FF:FF:FF"; 
};

struct PacketInjectionStats { 
    unsigned long packetsSent = 0; 
    unsigned long errors = 0; 
};

struct PingOfDeathStats { 
    unsigned long packetsSent = 0; 
    unsigned long errors = 0; 
};

// --- Wi-Fi Scanning Data ---
struct APInfo {
    String ssid;
    String bssid;
    int32_t rssi;
    uint8_t channel;
    String encryptionType;
    bool hidden;
    unsigned long lastSeen;
    bool operator==(const APInfo& other) const { return bssid == other.bssid; }
};

// --- PMKID Data ---
struct PMKIDInfo {
    String apMac;
    String clientMac;
    String pmkid;
    String apSsid;
    bool operator==(const PMKIDInfo& other) const {
        return apMac == other.apMac && clientMac == other.clientMac && pmkid == other.pmkid;
    }
};

// --- 802.11 Packet Structures ---
struct ieee80211_hdr {
    uint16_t frame_control;
    uint16_t duration_id;
    uint8_t addr1[6]; // DA or RA
    uint8_t addr2[6]; // SA or TA
    uint8_t addr3[6]; // BSSID
    uint16_t seq_ctrl;
} __attribute__((packed));

struct ieee80211_mgmt_beacon_fixed_params {
    uint64_t timestamp;
    uint16_t beacon_interval;
    uint16_t capability_info;
} __attribute__((packed));

struct ieee80211_deauth_frame {
    uint16_t frame_control;
    uint16_t duration_id;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint16_t reason_code;
} __attribute__((packed));

struct beacon_frame_s {
    ieee80211_hdr hdr;
    ieee80211_mgmt_beacon_fixed_params fixed_params;
    uint8_t ssid_tag_id;
    uint8_t ssid_tag_len;
    char ssid_data[32];
    uint8_t rates_tag_id;
    uint8_t rates_tag_len;
    uint8_t rates_data[8];
    uint8_t ds_param_tag_id;
    uint8_t ds_param_tag_len;
    uint8_t ds_param_channel;
} __attribute__((packed));

struct probe_request_frame_s {
    ieee80211_hdr hdr;
    uint8_t ssid_tag_id;
    uint8_t ssid_tag_len;
    char ssid_data[33]; // Max 32 for SSID + 1 for null terminator if needed
    uint8_t rates_tag_id;
    uint8_t rates_tag_len;
    uint8_t rates_data[8];
} __attribute__((packed));

struct ieee80211_ack_frame {
    uint16_t frame_control;
    uint16_t duration_id;
    uint8_t addr1_ra[6];
} __attribute__((packed));

#endif // DATA_STRUCTURES_H

