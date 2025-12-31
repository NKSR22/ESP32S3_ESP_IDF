#pragma once

#include <string>
#include <vector>
#include "esp_wifi.h"

class Network {
public:
    Network();
    void init();
    void update(); // Handle async tasks

    // Wi-Fi
    void startScan();
    void connect(std::string ssid, std::string password);
    bool isConnected();
    bool isScanning();
    std::vector<std::string> getScanResults();

    // NTP
    void syncTime();

private:
    bool _isConnected;
    bool _isScanning;

    // ESP-IDF specific handles or state variables
    static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data);
};
