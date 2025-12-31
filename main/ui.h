#pragma once

#include "globals.h"
#include <string>
#include <vector>

class UI {
public:
    UI();
    void init();
    void update(); // Main UI loop
    void handleTouch(TouchPoint p);

    // State management
    void changeState(AppState newState);
    AppState getState();

    // Specific screen updates
    void drawClock();
    void drawSettings();
    void drawWifiScan();
    void drawKeyboard();
    void drawStatusBar();

    // Helpers
    void setBrightness(int level); // 10-100
    void updateWifiList(const std::vector<std::string>& ssids);

private:
    AppState currentState;
    int brightness;

    // Placeholder for LVGL or GFX objects
    // lv_obj_t* currentScreen;
};
