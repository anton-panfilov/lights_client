#include <EEPROM.h>

void WiFiCredentials::loadSSID() {
    static char ssid[maxSSIDLength + 1];
    for (int i = 0; i < maxSSIDLength; i++) {
        ssid[i] = EEPROM.read(i);
    }
    ssid[maxSSIDLength] = '\0';
}

void WiFiCredentials::loadPassword() {
    static char password[maxPasswordLength + 1];
    for (int i = 0; i < maxPasswordLength; i++) {
        password[i] = EEPROM.read(i + maxSSIDLength + 1);
    }
    password[maxPasswordLength] = '\0';
}

void WiFiCredentials::saveSSID(const char *ssid) {
    for (int i = 0; i < maxSSIDLength; i++) {
        EEPROM.write(i, ssid[i]);
    }
    EEPROM.commit();
}

void WiFiCredentials::savePassword(const char *password) {
    for (int i = 0; i < maxPasswordLength; i++) {
        EEPROM.write(i + maxSSIDLength + 1, password[i]);
    }
    EEPROM.commit();
}