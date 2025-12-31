#pragma once

#include <string>

class Settings {
public:
    Settings();
    void load();
    void save();

    // Getters/Setters
    std::string getSSID();
    void setSSID(std::string ssid);

    std::string getPassword();
    void setPassword(std::string password);

    int getBrightness();
    void setBrightness(int level);

private:
    std::string _ssid;
    std::string _password;
    int _brightness;
};
