#pragma once

#include <stdint.h>
#include "driver/gpio.h"

// ==========================================
// System Configuration
// ==========================================
#define ADMIN_CODE "88888888"

// ==========================================
// Pin Definitions
// ==========================================

// I2C Pins (Touch, RTC, PMU, IO Expander)
// Prompt mentions RTC is on IO7/IO8.
// Assuming IO7 = SDA, IO8 = SCL (Needs verification with schematic, but using as placeholder)
#define PIN_I2C_SDA     GPIO_NUM_7
#define PIN_I2C_SCL     GPIO_NUM_8

// Display (ST7796 SPI)
// Please configure these according to your specific board schematic
#define PIN_LCD_MOSI    GPIO_NUM_11
#define PIN_LCD_SCLK    GPIO_NUM_12
#define PIN_LCD_CS      GPIO_NUM_10
#define PIN_LCD_DC      GPIO_NUM_13
#define PIN_LCD_MISO    GPIO_NUM_NC // Often not used for display only
#define PIN_LCD_BL      GPIO_NUM_14 // Backlight

// Touch (FT6336)
// Shares I2C Bus
// Interrupt and Reset might be on IO Expander or direct GPIO
// The prompt says TP_INT, RTC_INT, LCD_RST are on IO Expander (EXIO)

// ==========================================
// IO Expander (EXIO) Configuration
// ==========================================
// Assuming PCA9555 or similar on the same I2C bus
#define EXIO_I2C_ADDR   0x20 // Example address
// Bit masks for EXIO pins (hypothetical)
#define EXIO_PIN_LCD_RST  (1 << 0)
#define EXIO_PIN_TP_INT   (1 << 1)
#define EXIO_PIN_RTC_INT  (1 << 2)

// ==========================================
// State Machine
// ==========================================
enum class AppState {
    CLOCK,
    SETTINGS,
    WIFI_SCAN,
    KEYBOARD
};

// ==========================================
// Global Constants
// ==========================================
#define WIFI_SCAN_INTERVAL_MS 10000

// ==========================================
// Global Object References (Forward Declarations)
// ==========================================
// Check globals.cpp for definitions if needed,
// or main.cpp if they are defined there.

class UI;
class Network;
class Settings;

extern UI* ui;
extern Network* network;
extern Settings* settings;

// ==========================================
// Utility Structures
// ==========================================
struct TouchPoint {
    int x;
    int y;
    bool touched;
};
