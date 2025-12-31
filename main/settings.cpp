#include "settings.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <cstdio>
#include "esp_log.h"

static const char* TAG = "Settings";

Settings::Settings() {
    _ssid = "";
    _password = "";
    _brightness = 100;
}

void Settings::load() {
    ESP_LOGI(TAG, "Loading settings from NVS...");
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return;
    }

    size_t required_size;
    // Load SSID
    if (nvs_get_str(my_handle, "ssid", NULL, &required_size) == ESP_OK) {
        char* ssid_buf = new char[required_size];
        nvs_get_str(my_handle, "ssid", ssid_buf, &required_size);
        _ssid = std::string(ssid_buf);
        delete[] ssid_buf;
    }

    // Load Password
    if (nvs_get_str(my_handle, "password", NULL, &required_size) == ESP_OK) {
        char* pass_buf = new char[required_size];
        nvs_get_str(my_handle, "password", pass_buf, &required_size);
        _password = std::string(pass_buf);
        delete[] pass_buf;
    }

    // Load Brightness
    int32_t b_val = 0;
    if (nvs_get_i32(my_handle, "brightness", &b_val) == ESP_OK) {
        _brightness = (int)b_val;
    }

    nvs_close(my_handle);
}

void Settings::save() {
    ESP_LOGI(TAG, "Saving settings to NVS...");
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    nvs_set_str(my_handle, "ssid", _ssid.c_str());
    nvs_set_str(my_handle, "password", _password.c_str());
    nvs_set_i32(my_handle, "brightness", _brightness);

    nvs_commit(my_handle);
    nvs_close(my_handle);
}

std::string Settings::getSSID() { return _ssid; }
void Settings::setSSID(std::string ssid) { _ssid = ssid; }

std::string Settings::getPassword() { return _password; }
void Settings::setPassword(std::string password) { _password = password; }

int Settings::getBrightness() { return _brightness; }
void Settings::setBrightness(int level) { _brightness = level; }
