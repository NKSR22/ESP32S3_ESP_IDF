#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#include "globals.h"
#include "ui.h"
#include "network.h"
#include "settings.h"

// ==========================================
// Global Instances
// ==========================================
UI* ui = nullptr;
Network* network = nullptr;
Settings* settings = nullptr;

static const char* TAG = "Main";

// ==========================================
// Hardware Initialization
// ==========================================
void init_hardware() {
    ESP_LOGI(TAG, "Initializing Hardware...");

    // I2C Init
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = PIN_I2C_SDA,
        .scl_io_num = PIN_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000
        },
        .clk_flags = 0
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);

    // TODO: Init PMU (AXP2101)
    // TODO: Init IO Expander
    // TODO: Init RTC
}

// ==========================================
// Main Logic
// ==========================================
extern "C" void app_main(void)
{
    // 1. Initialize Global Objects
    settings = new Settings();
    ui = new UI();
    network = new Network();

    // 2. Load Settings
    settings->load();
    ui->setBrightness(settings->getBrightness());

    // 3. Initialize Hardware & Drivers
    init_hardware();
    ui->init();
    network->init();

    // 4. Connect to Wi-Fi if available
    if (!settings->getSSID().empty()) {
        network->connect(settings->getSSID(), settings->getPassword());
    }

    // 5. Main Loop
    while (true) {
        // UI Update Loop
        ui->update();

        // Network Async Check
        network->update();

        // Touch Input Handling (Placeholder)
        // In reality, this might be interrupt-driven or polled via I2C
        // TouchPoint p = touchDriver.getPoint();
        // ui->handleTouch(p);

        // Delay to yield to FreeRTOS
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
