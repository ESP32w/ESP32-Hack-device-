# ESP32 Interactive Hacking Toolkit V2

**Version: 2.0.0** (Enhanced with Detailed Stats, Graphs, and Configurable Attacks)

*Read this in [Russian](#esp32-interactive-hacking-toolkit-v2-1) / Читать на [русском](#esp32-interactive-hacking-toolkit-v2-1)*

## Project Overview

The ESP32 Interactive Hacking Toolkit is a comprehensive security testing platform designed for ethical penetration testing and security research. Built on the powerful ESP32 microcontroller, this toolkit combines web exploitation utilities and Wi-Fi network attack tools in a single, portable device. The entire system is managed through a dynamic and interactive web interface served directly by the ESP32, eliminating the need for additional hardware or software.

The toolkit serves as an educational resource for cybersecurity professionals, researchers, and enthusiasts who wish to understand common web vulnerabilities and wireless network security issues. By providing hands-on experience with various security testing techniques, users can better understand potential threats and develop more effective protection strategies.

With version 2.0.0, the toolkit has been significantly enhanced with detailed statistics, graphical data representation, and fully configurable attack parameters, making it more powerful and user-friendly than ever before.

## Key Features

The ESP32 Interactive Hacking Toolkit offers a wide range of features divided into two main categories: Internet Utilities and Wi-Fi Utilities, all accessible through an intuitive web interface.

### Interactive Web Interface

The toolkit provides a user-friendly graphical interface with a clear mode selection between "Интернет Утилиты" (Internet Utilities) and "Wi-Fi Утилиты" (Wi-Fi Utilities). The interface features a dark theme with green text, creating a consistent and visually appealing user experience reminiscent of classic hacking aesthetics.

In the Internet Utilities section, users can input a target URL for analysis. The ESP32 backend performs comprehensive security scans, including tests for Cross-Site Scripting (XSS), SQL Injection (SQLi), Local File Inclusion (LFI), and hidden administrative panels. Any potential vulnerabilities discovered are immediately displayed in the interface, allowing for further investigation.

The Wi-Fi Utilities section has been significantly enhanced in version 2.0.0. It now provides detailed Wi-Fi scanning capabilities that display comprehensive network information including SSID, BSSID, RSSI, channel, encryption type, manufacturer identification (based on OUI), estimated client count, and hidden network detection. The data is presented not only in tabular format but also through graphical representations, including an RSSI bar chart for signal strengths and an encryption type pie chart for network security distribution analysis.

### Configurable Wi-Fi Attack Modules

Version 2.0.0 introduces fully configurable Wi-Fi attack modules directly accessible from the web interface. These include:

- **Deauthentication Attack**: Allows for targeted disconnection of devices from wireless networks with configurable parameters such as packet count, interval, and duration.
- **Beacon Spam Attack**: Creates multiple fake access points with customizable settings for the number of SSIDs and packet intervals.
- **Probe Flood Attack**: Simulates devices searching for networks with adjustable packet count and timing parameters.
- **ACK Flood Attack**: Generates acknowledgment packets with configurable target specifics and timing settings.

All attack modules now feature real-time status reporting, providing immediate feedback on the operation's progress and effectiveness.

### Modular C++ Utilities

The toolkit is built with a modular architecture, making it highly extensible and maintainable. The core functionality is implemented through specialized C++ classes:

- **Web Exploit Scanners**: XSSTester, FormBrute, LFIRFIScanner, AdminPanelFinder, and SQLInjectionScanner provide comprehensive web vulnerability assessment capabilities.
- **Wi-Fi Attack Modules**: DeauthAttack, BeaconSpamAttack, ProbeFloodAttack, and AckFloodAttack implement wireless network security testing functions with enhanced parameter structures to accept detailed settings.

### Backend API on ESP32

The toolkit features a robust API system implemented in the main Arduino sketch. This API handles requests from the web interface, manages configurable attack parameters, and provides detailed scan data. The API endpoints are designed for efficient communication between the frontend and backend components, ensuring smooth operation even with limited resources.

### SPIFFS/LittleFS Storage

All web interface files, including HTML, CSS, and JavaScript (including Chart.js for graphical representations), are served directly from the ESP32's flash memory using either SPIFFS or LittleFS file systems. This approach eliminates the need for external storage while providing a responsive and feature-rich user experience.

## Disclaimer

This toolkit is intended for educational and ethical security testing purposes only. Unauthorized use against networks or systems for which you do not have explicit permission is illegal and unethical. The developers assume no liability and are not responsible for any misuse or damage caused by this program. Always ensure you have proper authorization before conducting any security tests.

## Project Structure

The ESP32 Interactive Hacking Toolkit follows a well-organized directory structure designed for clarity and ease of use with the Arduino IDE:

```
ESP32_Interactive_Hacking_Toolkit/ (Root of the project)
|-- ESP32_Interactive_Hacking_Toolkit.ino  (Main Arduino sketch with web server & API logic)
|-- src/                                   (Source files for C++ utilities)
|   |-- web_exploits/
|   |   |-- xss_tester.h
|   |   |-- xss_tester.cpp
|   |   |-- form_brute.h
|   |   |-- form_brute.cpp
|   |   |-- lfi_rfi_scanner.h
|   |   |-- lfi_rfi_scanner.cpp
|   |   |-- admin_panel_finder.h
|   |   |-- admin_panel_finder.cpp
|   |   |-- sql_injection_scanner.h
|   |   |-- sql_injection_scanner.cpp
|   |   `-- web_exploit_config.h
|   `-- wifi_attacks/
|       |-- ack_flood_attack.h
|       |-- ack_flood_attack.cpp
|       |-- beacon_spam_attack.h
|       |-- beacon_spam_attack.cpp
|       |-- deauth_attack.h
|       |-- deauth_attack.cpp
|       |-- probe_flood_attack.h
|       |-- probe_flood_attack.cpp
|       `-- data_structures.h
|-- data/                                  (Web interface files for SPIFFS/LittleFS)
|   |-- index.html
|   |-- dark_theme.css
|   |-- interactive_style.css
|   `-- interactive_script.js              (Now includes Chart.js integration)
|-- docs/                                  (Additional documentation)
|   |-- project_report.md
|   |-- architecture_design.md
|   `-- technical_analysis_enhancements.md (Details on new features)
|-- LICENSE                                (Project License - e.g., MIT)
|-- README.md                              (This file)
`-- .gitignore                             (Specifies intentionally untracked files)
```

This structure separates the core functionality (in the src directory) from the web interface files (in the data directory) and documentation (in the docs directory), making it easy to navigate and maintain the project.

## Prerequisites

Before setting up the ESP32 Interactive Hacking Toolkit, ensure you have the following hardware and software components:

### Hardware Requirements

- **ESP32 Development Board**: Compatible models include ESP32-WROOM-32, ESP32-S2, and ESP32-S3. The toolkit has been tested and optimized for these variants, but may work with other ESP32-based boards as well.

### Software Requirements

- **Arduino IDE**: Version 1.8.13 or newer is recommended for optimal compatibility. Alternatively, PlatformIO can be used for development.
- **ESP32 Board Support Package**: This must be installed in the Arduino IDE through the Boards Manager. It provides the core functionality needed to program ESP32 devices.
- **ESP32 Sketch Data Upload Tool**: This tool is essential for uploading the web interface files to the ESP32's SPIFFS or LittleFS file system. Search for "ESP32FS" or "LittleFS_esp32" plugin for Arduino IDE to install it.

### Required Libraries

The following libraries must be installed via the Arduino Library Manager:

- `WiFi.h` (usually included as part of the ESP32 core)
- `FS.h` & `SPIFFS.h` (or `LittleFS.h`) (part of the ESP32 core)
- `ESPAsyncWebServer` by Hristo Gochkov (Recommended for non-blocking server operation)
- `AsyncTCP` by Hristo Gochkov (Required dependency for ESPAsyncWebServer)
- `ArduinoJson` by Benoit Blanchon (Used for parsing and generating JSON API responses)

## Setup and Installation

Follow these steps to set up the ESP32 Interactive Hacking Toolkit on your device:

1. **Clone or Download Repository**: 
   Obtain the project files by cloning this repository or downloading it as a ZIP archive and extracting it to your local machine.

2. **Install Arduino IDE and ESP32 Core**: 
   If you haven't already, download and install the Arduino IDE from the official website. Then, add ESP32 board support by going to Preferences, adding `https://dl.espressif.com/dl/package_esp32_index.json` to the "Additional Boards Manager URLs" field, and installing the ESP32 package from the Boards Manager.

3. **Install Required Libraries**: 
   Open Arduino IDE, navigate to Sketch > Include Library > Manage Libraries, and install the following libraries:
   - ESPAsyncWebServer
   - AsyncTCP
   - ArduinoJson

4. **Install Filesystem Uploader**: 
   Find and install the appropriate ESP32 Sketch Data Upload tool for your Arduino IDE version. This is typically available as a plugin that needs to be manually installed.

5. **Open Project in Arduino IDE**: 
   Open the `ESP32_Interactive_Hacking_Toolkit.ino` file in Arduino IDE. This is the main sketch that contains the core functionality of the toolkit.

6. **Select Board and Port**: 
   In Arduino IDE, select your specific ESP32 board model from the Tools > Board menu and choose the correct COM port from Tools > Port.

7. **Configure Wi-Fi (Optional)**: 
   By default, the toolkit operates in Access Point (AP) mode with the SSID `ESP32_Hacking_Tool` and password `password123`. If you wish to modify these settings, you can do so in the main `.ino` sketch.

## Compilation and Upload

After completing the setup, follow these steps to compile and upload the toolkit to your ESP32 device:

1. **Upload Filesystem Image**:
   - Ensure all web interface files (`index.html`, `dark_theme.css`, `interactive_style.css`, `interactive_script.js`) are placed in a folder named `data` within your Arduino sketch directory.
   - In Arduino IDE, click Tools > ESP32 Sketch Data Upload to transfer these files to the ESP32's flash memory.
   - Wait for the upload process to complete. This may take several minutes depending on the size of the files.

2. **Compile and Upload Sketch**:
   - Click the "Upload" button in the Arduino IDE (or use Ctrl+U) to compile the sketch and upload it to your ESP32 device.
   - Monitor the output in the Serial Monitor to ensure the upload is successful and the toolkit initializes correctly.

## Using the Toolkit

Once the toolkit is installed on your ESP32 device, you can begin using its features:

1. **Power On ESP32 & Connect**:
   - Power on your ESP32 device.
   - Connect to the ESP32's Wi-Fi network (default SSID: `ESP32_Hacking_Tool`, password: `password123`) using your computer, smartphone, or tablet.

2. **Access Web Interface**:
   - Open a web browser and navigate to `http://192.168.4.1` (the default IP address when operating in AP mode).
   - If you've configured the ESP32 to connect to an existing network (STA mode), use the IP address assigned to the ESP32 by your router.

3. **Interact with the Interface**:
   - **Choose Mode**: Select either "Интернет Утилиты" (Internet Utilities) or "Wi-Fi Утилиты" (Wi-Fi Utilities) based on your testing needs.
   
   - **Internet Utilities**:
     - Enter a target URL in the provided field.
     - Click "Анализировать" (Analyze) to begin the security assessment.
     - Review the results displayed in the interface, which will show potential vulnerabilities detected in the target website.
   
   - **Wi-Fi Utilities**:
     - Click the scan button to discover nearby wireless networks.
     - Review the detailed information displayed in the table, including SSID, signal strength, encryption type, and manufacturer.
     - Examine the graphical representations of RSSI values and encryption types for a visual analysis of the wireless environment.
     - To use the attack modules, fill in the specific parameters for your chosen attack (e.g., Target BSSID, Number of Packets, Interval, Duration).
     - Click the respective "Начать ..." (Start) button to initiate the attack.
     - Monitor the status of the attack in real-time.
     - Use the "Остановить ..." (Stop) button to terminate the attack when desired.

## API Endpoints

The web interface communicates with the ESP32 using the following API endpoints:

### Web Vulnerability Assessment

- **`POST /api/analyze_url`**:
  - **Purpose**: Analyzes a target URL for web vulnerabilities
  - **Request Body (JSON)**: `{"url": "<target_url>"}`
  - **Response (JSON)**: Detailed information about potential web vulnerabilities found

### Wi-Fi Network Scanning

- **`GET /api/wifi/scan_networks`**:
  - **Purpose**: Scans for available Wi-Fi networks with enhanced details
  - **Response (JSON)**:
    ```json
    {
      "success": true,
      "message": "X networks found",
      "networks": [
        {
          "ssid": "...", 
          "rssi": -50, 
          "bssid": "...", 
          "channel": 6, 
          "encryptionType": "WPA2-PSK",
          "manufacturer": "VendorX",
          "clients": 1,
          "isHidden": false
        }
      ],
      "hidden_count": 0
    }
    ```

### Wi-Fi Attack Controls

- **`POST /api/wifi/start_deauth`**:
  - **Purpose**: Starts a Deauthentication attack with specified settings
  - **Request Body (JSON)**: `{"target_bssid": "XX:XX:XX:XX:XX:XX", "target_client_mac": "YY:YY:YY:YY:YY:YY", "num_packets": 100, "packet_interval_ms": 100, "attack_duration_s": 0}`
  - **Response (JSON)**: `{"success": true, "message": "Deauth attack started..."}`

- **`POST /api/wifi/start_beacon_spam`**:
  - **Purpose**: Starts a Beacon spam attack with specified settings
  - **Request Body (JSON)**: `{"num_ssids": 10, "packet_interval_ms": 100, "encryption_type": 0}` (0 for Open, 1 for WPA-PSK)
  - **Response (JSON)**: `{"success": true, "message": "Beacon spam started..."}`

- **`POST /api/wifi/start_probe_flood`**:
  - **Purpose**: Starts a Probe Request flood with specified settings
  - **Request Body (JSON)**: `{"num_packets": 1000, "packet_interval_ms": 50, "attack_duration_s": 0}`
  - **Response (JSON)**: `{"success": true, "message": "Probe flood started..."}`

- **`POST /api/wifi/start_ack_flood`**:
  - **Purpose**: Starts an ACK flood attack with specified settings
  - **Request Body (JSON)**: `{"target_bssid": "XX:XX:XX:XX:XX:XX", "num_packets": 1000, "packet_interval_ms": 10, "attack_duration_s": 0}`
  - **Response (JSON)**: `{"success": true, "message": "ACK flood started..."}`

- **`POST /api/wifi/stop_deauth`**:
  - **Purpose**: Stops the Deauthentication attack
  - **Response (JSON)**: `{"success": true, "message": "Attack stopped..."}`

- **`POST /api/wifi/stop_beacon_spam`**:
  - **Purpose**: Stops the Beacon Spam attack
  - **Response (JSON)**: `{"success": true, "message": "Attack stopped..."}`

- **`POST /api/wifi/stop_probe_flood`**:
  - **Purpose**: Stops the Probe Flood attack
  - **Response (JSON)**: `{"success": true, "message": "Attack stopped..."}`

- **`POST /api/wifi/stop_ack_flood`**:
  - **Purpose**: Stops the ACK Flood attack
  - **Response (JSON)**: `{"success": true, "message": "Attack stopped..."}`

- **`GET /api/wifi/status`**:
  - **Purpose**: Retrieves the current status of all Wi-Fi attack modules
  - **Response (JSON)**: Status information for each attack type

## Troubleshooting

If you encounter issues while setting up or using the ESP32 Interactive Hacking Toolkit, consider the following troubleshooting steps:

- **Upload Failures**:
  - Ensure you have selected the correct board and port in Arduino IDE.
  - Try pressing the BOOT button on your ESP32 board during the upload process.
  - Check that you have sufficient permissions to access the COM port.

- **Web Interface Not Loading**:
  - Verify that the filesystem upload was successful.
  - Check the Serial Monitor for any error messages during initialization.
  - Ensure you are connected to the correct Wi-Fi network and using the right IP address.

- **Scan or Attack Functions Not Working**:
  - Monitor the Serial Monitor for error messages or exceptions.
  - Check for JSON parsing errors if you've modified any of the attack parameters.
  - Ensure your ESP32 has sufficient power; some operations may require more current than a standard USB port can provide.

- **Performance Issues**:
  - The ESP32 has limited resources; avoid running multiple intensive operations simultaneously.
  - Consider reducing the complexity of operations or increasing delays between requests if the system becomes unresponsive.

## Contributing

Contributions to the ESP32 Interactive Hacking Toolkit are welcome! If you'd like to contribute, please consider the following areas:

- **Bug Fixes**: Help identify and fix issues in the existing codebase.
- **Feature Enhancements**: Implement new features or improve existing ones.
- **Documentation**: Enhance the documentation with more examples, use cases, or translations.
- **Optimization**: Improve the performance and resource usage of the toolkit.
- **New Attack Modules**: Develop additional security testing modules.

To contribute, fork the repository, make your changes, and submit a pull request with a clear description of the modifications and their purpose.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

# ESP32 Interactive Hacking Toolkit V2

**Версия: 2.0.0** (Расширенная с детальной статистикой, графиками и настраиваемыми атаками)

*Read this in [English](#esp32-interactive-hacking-toolkit-v2) / Читать на [английском](#esp32-interactive-hacking-toolkit-v2)*

## Обзор проекта

ESP32 Interactive Hacking Toolkit представляет собой комплексную платформу для тестирования безопасности, разработанную для этичного пентестинга и исследований в области безопасности. Построенный на мощном микроконтроллере ESP32, этот набор инструментов объединяет утилиты для эксплуатации веб-уязвимостей и инструменты для атак на Wi-Fi сети в одном портативном устройстве. Вся система управляется через динамический и интерактивный веб-интерфейс, предоставляемый непосредственно ESP32, что исключает необходимость в дополнительном оборудовании или программном обеспечении.

Набор инструментов служит образовательным ресурсом для профессионалов в области кибербезопасности, исследователей и энтузиастов, которые хотят понять распространенные веб-уязвимости и проблемы безопасности беспроводных сетей. Предоставляя практический опыт работы с различными методами тестирования безопасности, пользователи могут лучше понять потенциальные угрозы и разработать более эффективные стратегии защиты.

В версии 2.0.0 набор инструментов был значительно улучшен с добавлением детальной статистики, графического представления данных и полностью настраиваемых параметров атак, что делает его более мощным и удобным для пользователя, чем когда-либо прежде.

## Ключевые особенности

ESP32 Interactive Hacking Toolkit предлагает широкий спектр функций, разделенных на две основные категории: Интернет-утилиты и Wi-Fi-утилиты, все доступные через интуитивно понятный веб-интерфейс.

### Интерактивный веб-интерфейс

Набор инструментов предоставляет удобный графический интерфейс с четким выбором режима между "Интернет Утилиты" и "Wi-Fi Утилиты". Интерфейс выполнен в темной теме с зеленым текстом, создавая последовательный и визуально привлекательный пользовательский опыт, напоминающий классическую хакерскую эстетику.

В разделе Интернет-утилит пользователи могут ввести целевой URL для анализа. Бэкенд ESP32 выполняет комплексное сканирование безопасности, включая тесты на межсайтовый скриптинг (XSS), SQL-инъекции (SQLi), локальное включение файлов (LFI) и скрытые административные панели. Любые обнаруженные потенциальные уязвимости немедленно отображаются в интерфейсе, позволяя провести дальнейшее исследование.

Раздел Wi-Fi-утилит был значительно улучшен в версии 2.0.0. Теперь он предоставляет детальные возможности сканирования Wi-Fi, которые отображают комплексную информацию о сети, включая SSID, BSSID, RSSI, канал, тип шифрования, идентификацию производителя (на основе OUI), оценочное количество клиентов и обнаружение скрытых сетей. Данные представлены не только в табличном формате, но и через графические представления, включая столбчатую диаграмму RSSI для уровней сигнала и круговую диаграмму типов шифрования для анализа распределения безопасности сети.

### Настраиваемые модули атак Wi-Fi

Версия 2.0.0 представляет полностью настраиваемые модули атак Wi-Fi, доступные непосредственно из веб-интерфейса. Они включают:

- **Атака деаутентификации**: Позволяет целенаправленно отключать устройства от беспроводных сетей с настраиваемыми параметрами, такими как количество пакетов, интервал и продолжительность.
- **Атака спама маяками**: Создает несколько поддельных точек доступа с настраиваемыми параметрами для количества SSID и интервалов пакетов.
- **Атака флуда пробными запросами**: Имитирует устройства, ищущие сети, с регулируемым количеством пакетов и параметрами времени.
- **Атака ACK-флуда**: Генерирует пакеты подтверждения с настраиваемыми целевыми характеристиками и параметрами времени.

Все модули атак теперь имеют отчетность о состоянии в реальном времени, предоставляя немедленную обратную связь о ходе и эффективности операции.

### Модульные утилиты C++

Набор инструментов построен с модульной архитектурой, что делает его высоко расширяемым и поддерживаемым. Основная функциональность реализована через специализированные классы C++:

- **Сканеры веб-эксплойтов**: XSSTester, FormBrute, LFIRFIScanner, AdminPanelFinder и SQLInjectionScanner предоставляют комплексные возможности оценки веб-уязвимостей.
- **Модули атак Wi-Fi**: DeauthAttack, BeaconSpamAttack, ProbeFloodAttack и AckFloodAttack реализуют функции тестирования безопасности беспроводных сетей с расширенными структурами параметров для приема детальных настроек.

### Бэкенд API на ESP32

Набор инструментов имеет надежную систему API, реализованную в основном скетче Arduino. Этот API обрабатывает запросы от веб-интерфейса, управляет настраиваемыми параметрами атак и предоставляет детальные данные сканирования. Конечные точки API разработаны для эффективной коммуникации между компонентами фронтенда и бэкенда, обеспечивая плавную работу даже при ограниченных ресурсах.

### Хранилище SPIFFS/LittleFS

Все файлы веб-интерфейса, включая HTML, CSS и JavaScript (включая Chart.js для графических представлений), предоставляются непосредственно из флэш-памяти ESP32 с использованием файловых систем SPIFFS или LittleFS. Этот подход устраняет необходимость во внешнем хранилище, обеспечивая при этом отзывчивый и функционально богатый пользовательский опыт.

## Отказ от ответственности

Этот набор инструментов предназначен только для образовательных целей и этичного тестирования безопасности. Несанкционированное использование против сетей или систем, на которые у вас нет явного разрешения, является незаконным и неэтичным. Разработчики не несут ответственности и не отвечают за любое неправильное использование или ущерб, причиненный этой программой. Всегда убедитесь, что у вас есть надлежащее разрешение перед проведением любых тестов безопасности.

## Структура проекта

ESP32 Interactive Hacking Toolkit следует хорошо организованной структуре каталогов, разработанной для ясности и удобства использования с Arduino IDE:

```
ESP32_Interactive_Hacking_Toolkit/ (Корень проекта)
|-- ESP32_Interactive_Hacking_Toolkit.ino  (Основной скетч Arduino с логикой веб-сервера и API)
|-- src/                                   (Исходные файлы для утилит C++)
|   |-- web_exploits/
|   |   |-- xss_tester.h
|   |   |-- xss_tester.cpp
|   |   |-- form_brute.h
|   |   |-- form_brute.cpp
|   |   |-- lfi_rfi_scanner.h
|   |   |-- lfi_rfi_scanner.cpp
|   |   |-- admin_panel_finder.h
|   |   |-- admin_panel_finder.cpp
|   |   |-- sql_injection_scanner.h
|   |   |-- sql_injection_scanner.cpp
|   |   `-- web_exploit_config.h
|   `-- wifi_attacks/
|       |-- ack_flood_attack.h
|       |-- ack_flood_attack.cpp
|       |-- beacon_spam_attack.h
|       |-- beacon_spam_attack.cpp
|       |-- deauth_attack.h
|       |-- deauth_attack.cpp
|       |-- probe_flood_attack.h
|       |-- probe_flood_attack.cpp
|       `-- data_structures.h
|-- data/                                  (Файлы веб-интерфейса для SPIFFS/LittleFS)
|   |-- index.html
|   |-- dark_theme.css
|   |-- interactive_style.css
|   `-- interactive_script.js              (Теперь включает интеграцию Chart.js)
|-- docs/                                  (Дополнительная документация)
|   |-- project_report.md
|   |-- architecture_design.md
|   `-- technical_analysis_enhancements.md (Детали новых функций)
|-- LICENSE                                (Лицензия проекта - например, MIT)
|-- README.md                              (Этот файл)
`-- .gitignore                             (Указывает намеренно неотслеживаемые файлы)
```

Эта структура отделяет основную функциональность (в каталоге src) от файлов веб-интерфейса (в каталоге data) и документации (в каталоге docs), что облегчает навигацию и поддержку проекта.

## Предварительные требования

Перед настройкой ESP32 Interactive Hacking Toolkit убедитесь, что у вас есть следующие компоненты оборудования и программного обеспечения:

### Требования к оборудованию

- **Плата разработки ESP32**: Совместимые модели включают ESP32-WROOM-32, ESP32-S2 и ESP32-S3. Набор инструментов был протестирован и оптимизирован для этих вариантов, но может работать и с другими платами на базе ESP32.

### Требования к программному обеспечению

- **Arduino IDE**: Рекомендуется версия 1.8.13 или новее для оптимальной совместимости. В качестве альтернативы для разработки можно использовать PlatformIO.
- **Пакет поддержки плат ESP32**: Он должен быть установлен в Arduino IDE через Менеджер плат. Он предоставляет основную функциональность, необходимую для программирования устройств ESP32.
- **Инструмент загрузки данных скетча ESP32**: Этот инструмент необходим для загрузки файлов веб-интерфейса в файловую систему SPIFFS или LittleFS ESP32. Для установки ищите плагин "ESP32FS" или "LittleFS_esp32" для Arduino IDE.

### Необходимые библиотеки

Следующие библиотеки должны быть установлены через Менеджер библиотек Arduino:

- `WiFi.h` (обычно включается как часть ядра ESP32)
- `FS.h` и `SPIFFS.h` (или `LittleFS.h`) (часть ядра ESP32)
- `ESPAsyncWebServer` от Hristo Gochkov (рекомендуется для неблокирующей работы сервера)
- `AsyncTCP` от Hristo Gochkov (необходимая зависимость для ESPAsyncWebServer)
- `ArduinoJson` от Benoit Blanchon (используется для парсинга и генерации ответов JSON API)

## Настройка и установка

Следуйте этим шагам для настройки ESP32 Interactive Hacking Toolkit на вашем устройстве:

1. **Клонирование или загрузка репозитория**: 
   Получите файлы проекта, клонировав этот репозиторий или загрузив его как ZIP-архив и распаковав на ваш локальный компьютер.

2. **Установка Arduino IDE и ядра ESP32**: 
   Если вы еще не сделали этого, загрузите и установите Arduino IDE с официального сайта. Затем добавьте поддержку плат ESP32, перейдя в Preferences, добавив `https://dl.espressif.com/dl/package_esp32_index.json` в поле "Additional Boards Manager URLs", и установив пакет ESP32 из Менеджера плат.

3. **Установка необходимых библиотек**: 
   Откройте Arduino IDE, перейдите в Sketch > Include Library > Manage Libraries и установите следующие библиотеки:
   - ESPAsyncWebServer
   - AsyncTCP
   - ArduinoJson

4. **Установка загрузчика файловой системы**: 
   Найдите и установите соответствующий инструмент загрузки данных скетча ESP32 для вашей версии Arduino IDE. Обычно он доступен как плагин, который нужно установить вручную.

5. **Открытие проекта в Arduino IDE**: 
   Откройте файл `ESP32_Interactive_Hacking_Toolkit.ino` в Arduino IDE. Это основной скетч, который содержит основную функциональность набора инструментов.

6. **Выбор платы и порта**: 
   В Arduino IDE выберите вашу конкретную модель платы ESP32 из меню Tools > Board и выберите правильный COM-порт из Tools > Port.

7. **Настройка Wi-Fi (опционально)**: 
   По умолчанию набор инструментов работает в режиме точки доступа (AP) с SSID `ESP32_Hacking_Tool` и паролем `password123`. Если вы хотите изменить эти настройки, вы можете сделать это в основном скетче `.ino`.

## Компиляция и загрузка

После завершения настройки следуйте этим шагам для компиляции и загрузки набора инструментов на ваше устройство ESP32:

1. **Загрузка образа файловой системы**:
   - Убедитесь, что все файлы веб-интерфейса (`index.html`, `dark_theme.css`, `interactive_style.css`, `interactive_script.js`) размещены в папке с именем `data` внутри каталога вашего скетча Arduino.
   - В Arduino IDE нажмите Tools > ESP32 Sketch Data Upload для передачи этих файлов во флэш-память ESP32.
   - Дождитесь завершения процесса загрузки. Это может занять несколько минут в зависимости от размера файлов.

2. **Компиляция и загрузка скетча**:
   - Нажмите кнопку "Upload" в Arduino IDE (или используйте Ctrl+U) для компиляции скетча и загрузки его на ваше устройство ESP32.
   - Следите за выводом в Serial Monitor, чтобы убедиться, что загрузка прошла успешно и набор инструментов инициализируется правильно.

## Использование набора инструментов

После установки набора инструментов на ваше устройство ESP32 вы можете начать использовать его функции:

1. **Включение ESP32 и подключение**:
   - Включите ваше устройство ESP32.
   - Подключитесь к сети Wi-Fi ESP32 (по умолчанию SSID: `ESP32_Hacking_Tool`, пароль: `password123`) с помощью вашего компьютера, смартфона или планшета.

2. **Доступ к веб-интерфейсу**:
   - Откройте веб-браузер и перейдите по адресу `http://192.168.4.1` (IP-адрес по умолчанию при работе в режиме AP).
   - Если вы настроили ESP32 для подключения к существующей сети (режим STA), используйте IP-адрес, назначенный ESP32 вашим роутером.

3. **Взаимодействие с интерфейсом**:
   - **Выбор режима**: Выберите либо "Интернет Утилиты", либо "Wi-Fi Утилиты" в зависимости от ваших потребностей тестирования.
   
   - **Интернет Утилиты**:
     - Введите целевой URL в предоставленное поле.
     - Нажмите "Анализировать" для начала оценки безопасности.
     - Просмотрите результаты, отображаемые в интерфейсе, которые покажут потенциальные уязвимости, обнаруженные на целевом веб-сайте.
   
   - **Wi-Fi Утилиты**:
     - Нажмите кнопку сканирования для обнаружения близлежащих беспроводных сетей.
     - Просмотрите подробную информацию, отображаемую в таблице, включая SSID, силу сигнала, тип шифрования и производителя.
     - Изучите графические представления значений RSSI и типов шифрования для визуального анализа беспроводной среды.
     - Для использования модулей атак заполните конкретные параметры для выбранной атаки (например, целевой BSSID, количество пакетов, интервал, продолжительность).
     - Нажмите соответствующую кнопку "Начать ..." для инициирования атаки.
     - Отслеживайте статус атаки в реальном времени.
     - Используйте кнопку "Остановить ..." для прекращения атаки, когда это необходимо.

## Конечные точки API

Веб-интерфейс взаимодействует с ESP32, используя следующие конечные точки API:

### Оценка веб-уязвимостей

- **`POST /api/analyze_url`**:
  - **Назначение**: Анализирует целевой URL на наличие веб-уязвимостей
  - **Тело запроса (JSON)**: `{"url": "<target_url>"}`
  - **Ответ (JSON)**: Подробная информация о найденных потенциальных веб-уязвимостях

### Сканирование Wi-Fi сетей

- **`GET /api/wifi/scan_networks`**:
  - **Назначение**: Сканирует доступные Wi-Fi сети с расширенными деталями
  - **Ответ (JSON)**:
    ```json
    {
      "success": true,
      "message": "Найдено X сетей",
      "networks": [
        {
          "ssid": "...", 
          "rssi": -50, 
          "bssid": "...", 
          "channel": 6, 
          "encryptionType": "WPA2-PSK",
          "manufacturer": "VendorX",
          "clients": 1,
          "isHidden": false
        }
      ],
      "hidden_count": 0
    }
    ```

### Управление Wi-Fi атаками

- **`POST /api/wifi/start_deauth`**:
  - **Назначение**: Запускает атаку деаутентификации с указанными настройками
  - **Тело запроса (JSON)**: `{"target_bssid": "XX:XX:XX:XX:XX:XX", "target_client_mac": "YY:YY:YY:YY:YY:YY", "num_packets": 100, "packet_interval_ms": 100, "attack_duration_s": 0}`
  - **Ответ (JSON)**: `{"success": true, "message": "Атака деаутентификации запущена..."}`

- **`POST /api/wifi/start_beacon_spam`**:
  - **Назначение**: Запускает атаку спама маяками с указанными настройками
  - **Тело запроса (JSON)**: `{"num_ssids": 10, "packet_interval_ms": 100, "encryption_type": 0}` (0 для открытой, 1 для WPA-PSK)
  - **Ответ (JSON)**: `{"success": true, "message": "Спам маяками запущен..."}`

- **`POST /api/wifi/start_probe_flood`**:
  - **Назначение**: Запускает флуд пробными запросами с указанными настройками
  - **Тело запроса (JSON)**: `{"num_packets": 1000, "packet_interval_ms": 50, "attack_duration_s": 0}`
  - **Ответ (JSON)**: `{"success": true, "message": "Флуд пробными запросами запущен..."}`

- **`POST /api/wifi/start_ack_flood`**:
  - **Назначение**: Запускает атаку ACK-флуда с указанными настройками
  - **Тело запроса (JSON)**: `{"target_bssid": "XX:XX:XX:XX:XX:XX", "num_packets": 1000, "packet_interval_ms": 10, "attack_duration_s": 0}`
  - **Ответ (JSON)**: `{"success": true, "message": "ACK-флуд запущен..."}`

- **`POST /api/wifi/stop_deauth`**:
  - **Назначение**: Останавливает атаку деаутентификации
  - **Ответ (JSON)**: `{"success": true, "message": "Атака остановлена..."}`

- **`POST /api/wifi/stop_beacon_spam`**:
  - **Назначение**: Останавливает атаку спама маяками
  - **Ответ (JSON)**: `{"success": true, "message": "Атака остановлена..."}`

- **`POST /api/wifi/stop_probe_flood`**:
  - **Назначение**: Останавливает флуд пробными запросами
  - **Ответ (JSON)**: `{"success": true, "message": "Атака остановлена..."}`

- **`POST /api/wifi/stop_ack_flood`**:
  - **Назначение**: Останавливает атаку ACK-флуда
  - **Ответ (JSON)**: `{"success": true, "message": "Атака остановлена..."}`

- **`GET /api/wifi/status`**:
  - **Назначение**: Получает текущий статус всех модулей Wi-Fi атак
  - **Ответ (JSON)**: Информация о статусе для каждого типа атаки

## Устранение неполадок

Если вы столкнулись с проблемами при настройке или использовании ESP32 Interactive Hacking Toolkit, рассмотрите следующие шаги по устранению неполадок:

- **Ошибки загрузки**:
  - Убедитесь, что вы выбрали правильную плату и порт в Arduino IDE.
  - Попробуйте нажать кнопку BOOT на вашей плате ESP32 во время процесса загрузки.
  - Проверьте, что у вас достаточно прав для доступа к COM-порту.

- **Веб-интерфейс не загружается**:
  - Убедитесь, что загрузка файловой системы прошла успешно.
  - Проверьте Serial Monitor на наличие сообщений об ошибках во время инициализации.
  - Убедитесь, что вы подключены к правильной сети Wi-Fi и используете правильный IP-адрес.

- **Функции сканирования или атаки не работают**:
  - Следите за Serial Monitor на наличие сообщений об ошибках или исключений.
  - Проверьте наличие ошибок парсинга JSON, если вы изменили какие-либо параметры атаки.
  - Убедитесь, что ваш ESP32 имеет достаточное питание; некоторые операции могут требовать больше тока, чем может обеспечить стандартный USB-порт.

- **Проблемы с производительностью**:
  - ESP32 имеет ограниченные ресурсы; избегайте одновременного запуска нескольких интенсивных операций.
  - Рассмотрите возможность уменьшения сложности операций или увеличения задержек между запросами, если система становится неотзывчивой.

## Вклад в проект

Вклады в ESP32 Interactive Hacking Toolkit приветствуются! Если вы хотите внести свой вклад, рассмотрите следующие области:

- **Исправление ошибок**: Помогите выявить и исправить проблемы в существующем коде.
- **Улучшение функций**: Реализуйте новые функции или улучшите существующие.
- **Документация**: Улучшите документацию с большим количеством примеров, случаев использования или переводов.
- **Оптимизация**: Улучшите производительность и использование ресурсов набора инструментов.
- **Новые модули атак**: Разработайте дополнительные модули тестирования безопасности.

Чтобы внести свой вклад, сделайте форк репозитория, внесите изменения и отправьте запрос на включение с четким описанием модификаций и их назначения.

## Лицензия

Этот проект лицензирован под лицензией MIT - см. файл [LICENSE](LICENSE) для деталей.
