#include "ui.h"
#include <cstdio> // For printf (debugging)

UI::UI() {
    currentState = AppState::CLOCK;
    brightness = 100;
}

void UI::init() {
    printf("UI: Initializing...\n");
    // Initialize Display Driver (ST7796) here
    // Initialize Touch Driver (FT6336) here
    // Initialize GUI Library (LVGL) here
}

void UI::update() {
    // This function is called in the main loop
    // It should handle rendering updates or LVGL task handler

    // Example: lv_timer_handler();

    // For now, just a placeholder state machine logic
    switch (currentState) {
        case AppState::CLOCK:
            drawClock();
            break;
        case AppState::SETTINGS:
            drawSettings();
            break;
        case AppState::WIFI_SCAN:
            drawWifiScan();
            break;
        case AppState::KEYBOARD:
            drawKeyboard();
            break;
    }
}

void UI::handleTouch(TouchPoint p) {
    if (!p.touched) return;

    printf("UI: Touch at %d, %d\n", p.x, p.y);

    // Simple logic to switch states for demonstration
    // In a real app, this would check button coordinates

    // Example: Tap top corner to go to settings
    if (currentState == AppState::CLOCK && p.y < 50 && p.x > 200) {
        changeState(AppState::SETTINGS);
    }
    else if (currentState == AppState::SETTINGS && p.y > 300) {
        changeState(AppState::CLOCK);
    }
}

void UI::changeState(AppState newState) {
    if (currentState == newState) return;

    printf("UI: Changing state to %d\n", (int)newState);
    currentState = newState;

    // Clear screen or prepare new screen
}

AppState UI::getState() {
    return currentState;
}

void UI::drawClock() {
    // Draw clock face, time, date
    // Use RTC data
}

void UI::drawSettings() {
    // Draw sliders for brightness, manual time set buttons
    // Draw "Scan Wifi" button
}

void UI::drawWifiScan() {
    // Draw list of Wi-Fi networks
}

void UI::drawKeyboard() {
    // Draw QWERTY keyboard for password entry
}

void UI::drawStatusBar() {
    // Draw Wi-Fi icon, Battery level, Time (small)
}

void UI::setBrightness(int level) {
    if (level < 10) level = 10;
    if (level > 100) level = 100;
    brightness = level;
    // Apply to PWM or PMU
    printf("UI: Brightness set to %d%%\n", brightness);
}

void UI::updateWifiList(const std::vector<std::string>& ssids) {
    // Update the list shown in WIFI_SCAN state
    printf("UI: Updated Wi-Fi list with %d networks\n", (int)ssids.size());
}
