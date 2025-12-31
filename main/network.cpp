#include "network.h"
#include <cstdio>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "globals.h"

// TAG for logging
static const char* TAG = "Network";

Network::Network() {
    _isConnected = false;
    _isScanning = false;
}

void Network::init() {
    ESP_LOGI(TAG, "Initializing Network...");

    // Initialize NVS (needed for Wi-Fi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Register Event Handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Network::wifi_event_handler,
                                                        this,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Network::wifi_event_handler,
                                                        this,
                                                        NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void Network::update() {
    // Check for scan completion or timeouts
}

void Network::startScan() {
    if (_isScanning) return;
    _isScanning = true;
    ESP_LOGI(TAG, "Starting Wi-Fi Scan...");

    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = true
    };
    esp_wifi_scan_start(&scan_config, false); // false = async (non-blocking)
}

void Network::connect(std::string ssid, std::string password) {
    ESP_LOGI(TAG, "Connecting to %s...", ssid.c_str());

    wifi_config_t wifi_config = {};
    // Copy SSID and Password
    // Note: strncpy is safer
    snprintf((char*)wifi_config.sta.ssid, 32, "%s", ssid.c_str());
    snprintf((char*)wifi_config.sta.password, 64, "%s", password.c_str());

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}

bool Network::isConnected() {
    return _isConnected;
}

bool Network::isScanning() {
    return _isScanning;
}

std::vector<std::string> Network::getScanResults() {
    // This should be called when scan is done
    uint16_t number = 10;
    wifi_ap_record_t ap_info[10];
    uint16_t ap_count = 0;

    esp_wifi_scan_get_ap_records(&number, ap_info);
    esp_wifi_scan_get_ap_num(&ap_count);

    std::vector<std::string> results;
    for (int i = 0; i < number && i < ap_count; i++) {
        results.push_back(std::string((char*)ap_info[i].ssid));
    }
    return results;
}

void Network::syncTime() {
    if (!_isConnected) return;
    ESP_LOGI(TAG, "Syncing NTP...");
    // Implement SNTP sync here
}

// Static Event Handler
void Network::wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    Network* net = (Network*)arg;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // esp_wifi_connect(); // Don't auto connect immediately if we want to scan first?
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        net->_isConnected = false;
        ESP_LOGI(TAG, "Wi-Fi Disconnected");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        net->_isConnected = true;
        ESP_LOGI(TAG, "Wi-Fi Connected! Got IP.");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        net->_isScanning = false;
        ESP_LOGI(TAG, "Wi-Fi Scan Done");
        // Notify UI to update list
        // ui->updateWifiList(net->getScanResults());
    }
}
