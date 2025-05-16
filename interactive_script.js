// interactive_script.js

document.addEventListener('DOMContentLoaded', () => {
    // --- General Page Elements ---
    const initialChoiceSection = document.getElementById('initial-choice');
    const internetSection = document.getElementById('internet-section');
    const wifiSection = document.getElementById('wifi-section');
    const btnInternetMode = document.getElementById('btn-internet-mode');
    const btnWifiMode = document.getElementById('btn-wifi-mode');

    // --- Internet Section Elements ---
    const urlInput = document.getElementById('url-input');
    const analyzeUrlBtn = document.getElementById('analyze-url-btn');
    const internetResultsDiv = document.getElementById('internet-results');

    // --- Wi-Fi Section Elements ---
    const scanWifiBtn = document.getElementById('scan-wifi-btn');
    const wifiScanTableBody = document.getElementById('wifi-scan-table-body');
    const hiddenNetworksCountSpan = document.getElementById('hidden-networks-count');
    let rssiChartInstance = null;
    let encryptionChartInstance = null;

    // Deauth Attack Elements
    const deauthBssidInput = document.getElementById('deauth-bssid');
    const deauthClientMacInput = document.getElementById('deauth-client-mac');
    const deauthPacketsInput = document.getElementById('deauth-packets');
    const deauthIntervalInput = document.getElementById('deauth-interval');
    const deauthDurationInput = document.getElementById('deauth-duration');
    const startDeauthBtn = document.getElementById('start-deauth-btn');
    const stopDeauthBtn = document.getElementById('stop-deauth-btn');
    const deauthStatusP = document.getElementById('deauth-status');

    // Beacon Spam Attack Elements
    const beaconSsidCountInput = document.getElementById('beacon-ssid-count');
    const beaconIntervalInput = document.getElementById('beacon-interval');
    const beaconEncryptionSelect = document.getElementById('beacon-encryption');
    const startBeaconBtn = document.getElementById('start-beacon-btn');
    const stopBeaconBtn = document.getElementById('stop-beacon-btn');
    const beaconStatusP = document.getElementById('beacon-status');

    // Probe Flood Attack Elements
    const probeRequestsInput = document.getElementById('probe-requests');
    const probeIntervalInput = document.getElementById('probe-interval');
    const probeDurationInput = document.getElementById('probe-duration');
    const startProbeBtn = document.getElementById('start-probe-btn');
    const stopProbeBtn = document.getElementById('stop-probe-btn');
    const probeStatusP = document.getElementById('probe-status');

    // ACK Flood Attack Elements
    const ackBssidInput = document.getElementById('ack-bssid');
    const ackPacketsInput = document.getElementById('ack-packets');
    const ackIntervalInput = document.getElementById('ack-interval');
    const ackDurationInput = document.getElementById('ack-duration');
    const startAckBtn = document.getElementById('start-ack-btn');
    const stopAckBtn = document.getElementById('stop-ack-btn');
    const ackStatusP = document.getElementById('ack-status');

    // --- Utility Functions ---
    async function apiPost(endpoint, data) {
        try {
            const response = await fetch(endpoint, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(data),
            });
            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`HTTP error ${response.status}: ${errorText}`);
            }
            return await response.json();
        } catch (error) {
            console.error(`Error in POST ${endpoint}:`, error);
            throw error;
        }
    }

    async function apiGet(endpoint) {
        try {
            const response = await fetch(endpoint);
            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`HTTP error ${response.status}: ${errorText}`);
            }
            return await response.json();
        } catch (error) {
            console.error(`Error in GET ${endpoint}:`, error);
            throw error;
        }
    }

    // --- Mode Selection --- 
    btnInternetMode.addEventListener('click', () => {
        initialChoiceSection.classList.add('hidden');
        internetSection.classList.remove('hidden');
        wifiSection.classList.add('hidden');
        internetResultsDiv.innerHTML = '<p>Введите URL и нажмите "Анализировать" для отображения потенциальных уязвимостей.</p>';
    });

    btnWifiMode.addEventListener('click', () => {
        initialChoiceSection.classList.add('hidden');
        wifiSection.classList.remove('hidden');
        internetSection.classList.add('hidden');
        // Optionally, trigger an initial Wi-Fi scan or load status
    });

    // --- Internet Section Logic ---
    analyzeUrlBtn.addEventListener('click', async () => {
        const url = urlInput.value.trim();
        if (!url) {
            internetResultsDiv.innerHTML = '<p style="color: #ffcc00;">Пожалуйста, введите URL-адрес.</p>';
            return;
        }
        internetResultsDiv.innerHTML = `<p>Анализ URL: <strong>${url}</strong>...</p><p>Отправка запроса на ESP32...</p>`;
        try {
            // Replace with actual API call to ESP32
            // const analysisResult = await apiPost('/api/analyze_url', { url: url }); 
            // For now, using placeholder from previous version
            setTimeout(() => displayPotentialVulnerabilities(url), 1000); 
        } catch (error) {
            internetResultsDiv.innerHTML = `<p style="color: #ff6666;">Ошибка анализа URL: ${error.message}</p>`;
        }
    });

    function displayPotentialVulnerabilities(url) { // Placeholder from previous version
        let resultsHTML = `<h3>Потенциальные уязвимости для: ${url}</h3>`;
        resultsHTML += "<p><em>(Это демонстрационные данные. Реальное сканирование будет выполняться на ESP32.)</em></p>";
        resultsHTML += "<ul>";
        const vulnerabilities = [
            { name: "Cross-Site Scripting (XSS)", description: "Позволяет злоумышленнику внедрять вредоносные скрипты.", test_instruction: "Попробуйте <code>&lt;script&gt;alert('XSS')&lt;/script&gt;</code>." },
            { name: "SQL Инъекция (SQLi)", description: "Позволяет выполнять произвольные SQL-запросы.", test_instruction: "Попробуйте <code>' OR '1'='1</code>." },
            { name: "Local File Inclusion (LFI)", description: "Позволяет читать локальные файлы.", test_instruction: "Попробуйте <code>?file=../../etc/passwd</code>." }
        ];
        vulnerabilities.forEach(vuln => {
            resultsHTML += `<li class="vulnerability-item"><strong>${vuln.name}</strong>: ${vuln.description}<br><small><em>Проверка: ${vuln.test_instruction}</em></small></li>`;
        });
        resultsHTML += "</ul>";
        internetResultsDiv.innerHTML = resultsHTML;
    }

    // --- Wi-Fi Section Logic ---
    scanWifiBtn.addEventListener('click', async () => {
        wifiScanTableBody.innerHTML = '<tr><td colspan="7">Сканирование...</td></tr>';
        try {
            const scanData = await apiGet('/api/wifi/scan_networks'); // ESP32 API endpoint
            renderWifiScanResults(scanData.networks || []);
            updateWifiAnalyticsCharts(scanData.networks || []);
            hiddenNetworksCountSpan.textContent = scanData.hidden_count || 0;
        } catch (error) {
            wifiScanTableBody.innerHTML = `<tr><td colspan="7" style="color: #ff6666;">Ошибка сканирования: ${error.message}</td></tr>`;
        }
    });

    function renderWifiScanResults(networks) {
        wifiScanTableBody.innerHTML = ''; // Clear previous results
        if (networks.length === 0) {
            wifiScanTableBody.innerHTML = '<tr><td colspan="7">Сети не найдены.</td></tr>';
            return;
        }
        networks.forEach(net => {
            const row = wifiScanTableBody.insertRow();
            row.insertCell().textContent = net.ssid || (net.isHidden ? '[Скрытая Сеть]' : 'N/A');
            row.insertCell().textContent = net.bssid || 'N/A';
            row.insertCell().textContent = net.rssi || 'N/A';
            row.insertCell().textContent = net.channel || 'N/A';
            row.insertCell().textContent = net.encryptionType || 'N/A';
            row.insertCell().textContent = net.manufacturer || 'N/A'; // From OUI lookup
            row.insertCell().textContent = net.clients !== undefined ? net.clients : 'N/A'; // Best effort
        });
    }

    function updateWifiAnalyticsCharts(networks) {
        if (!networks || networks.length === 0) return;

        // RSSI Chart
        const rssiLabels = networks.map(n => n.ssid || n.bssid);
        const rssiValues = networks.map(n => n.rssi);
        if (rssiChartInstance) rssiChartInstance.destroy();
        rssiChartInstance = new Chart(document.getElementById('rssi-chart').getContext('2d'), {
            type: 'bar',
            data: {
                labels: rssiLabels,
                datasets: [{
                    label: 'RSSI (dBm)',
                    data: rssiValues,
                    backgroundColor: 'rgba(0, 255, 0, 0.6)',
                    borderColor: 'rgba(0, 255, 0, 1)',
                    borderWidth: 1
                }]
            },
            options: { scales: { y: { beginAtZero: false } }, responsive: true, maintainAspectRatio: false }
        });

        // Encryption Type Chart
        const encryptionCounts = {};
        networks.forEach(n => {
            const type = n.encryptionType || 'Unknown';
            encryptionCounts[type] = (encryptionCounts[type] || 0) + 1;
        });
        if (encryptionChartInstance) encryptionChartInstance.destroy();
        encryptionChartInstance = new Chart(document.getElementById('encryption-chart').getContext('2d'), {
            type: 'doughnut',
            data: {
                labels: Object.keys(encryptionCounts),
                datasets: [{
                    label: 'Типы шифрования',
                    data: Object.values(encryptionCounts),
                    backgroundColor: ['rgba(0, 255, 0, 0.7)', 'rgba(0, 200, 0, 0.7)', 'rgba(0, 150, 0, 0.7)', 'rgba(0, 100, 0, 0.7)', 'rgba(50, 50, 50, 0.7)'],
                }]
            },
            options: { responsive: true, maintainAspectRatio: false }
        });
    }

    // --- Wi-Fi Attack Controls --- 
    function setupAttackControls(attackName, startBtn, stopBtn, statusP, settingsGetter, startEndpoint, stopEndpointSuffix) {
        startBtn.addEventListener('click', async () => {
            const settings = settingsGetter();
            if (!settings) return; // Validation failed in getter
            statusP.textContent = `Запуск ${attackName}...`;
            statusP.style.color = '#00FF00';
            startBtn.classList.add('hidden');
            stopBtn.classList.remove('hidden');
            try {
                const result = await apiPost(startEndpoint, settings);
                statusP.textContent = `${attackName} запущена: ${result.message || 'OK'}`;
            } catch (error) {
                statusP.textContent = `Ошибка запуска ${attackName}: ${error.message}`;
                statusP.style.color = '#FF6666';
                startBtn.classList.remove('hidden');
                stopBtn.classList.add('hidden');
            }
        });

        stopBtn.addEventListener('click', async () => {
            statusP.textContent = `Остановка ${attackName}...`;
            try {
                // Assuming a generic stop endpoint or specific ones
                const result = await apiPost(`/api/wifi/stop_${stopEndpointSuffix || attackName.toLowerCase().replace(' ', '_')}`, {});
                statusP.textContent = `${attackName} остановлена: ${result.message || 'OK'}`;
            } catch (error) {
                statusP.textContent = `Ошибка остановки ${attackName}: ${error.message}`;
                statusP.style.color = '#FF6666';
            } finally {
                startBtn.classList.remove('hidden');
                stopBtn.classList.add('hidden');
            }
        });
    }

    // Deauth Attack
    setupAttackControls('Deauth Атака', startDeauthBtn, stopDeauthBtn, deauthStatusP, () => {
        const bssid = deauthBssidInput.value.trim();
        if (!bssid) { alert('BSSID цели обязателен для Deauth атаки.'); return null; }
        return {
            target_bssid: bssid,
            target_client_mac: deauthClientMacInput.value.trim(),
            num_packets: parseInt(deauthPacketsInput.value),
            packet_interval_ms: parseInt(deauthIntervalInput.value),
            attack_duration_s: parseInt(deauthDurationInput.value)
        };
    }, '/api/wifi/start_deauth', 'deauth');

    // Beacon Spam Attack
    setupAttackControls('Beacon Spam Атака', startBeaconBtn, stopBeaconBtn, beaconStatusP, () => {
        return {
            num_ssids: parseInt(beaconSsidCountInput.value),
            packet_interval_ms: parseInt(beaconIntervalInput.value),
            encryption_type: beaconEncryptionSelect.value === "wpa_psk" ? 1 : 0 // 0 for Open, 1 for WPA_PSK (as per .h file)
        };
    }, '/api/wifi/start_beacon_spam', 'beacon_spam');

    // Probe Flood Attack
    setupAttackControls('Probe Flood Атака', startProbeBtn, stopProbeBtn, probeStatusP, () => {
        return {
            num_packets: parseInt(probeRequestsInput.value),
            packet_interval_ms: parseInt(probeIntervalInput.value),
            attack_duration_s: parseInt(probeDurationInput.value)
        };
    }, '/api/wifi/start_probe_flood', 'probe_flood');

    // ACK Flood Attack
    setupAttackControls('ACK Flood Атака', startAckBtn, stopAckBtn, ackStatusP, () => {
        const bssid = ackBssidInput.value.trim();
        if (!bssid) { alert('BSSID цели обязателен для ACK Flood атаки.'); return null; }
        return {
            target_bssid: bssid,
            num_packets: parseInt(ackPacketsInput.value),
            packet_interval_ms: parseInt(ackIntervalInput.value),
            attack_duration_s: parseInt(ackDurationInput.value)
        };
    }, '/api/wifi/start_ack_flood', 'ack_flood');

});

