#include <Preferences.h>

Preferences preferences;

void WiFiCredentials::init() {
    preferences.begin("wifiCredentials", false);
}

void WiFiCredentials::loadFromPersistentStorage() {
    String stored_ssid = preferences.getString("ssid", "");
    String stored_password = preferences.getString("password", "");

    stored_ssid.toCharArray(ssid, maxSSIDLength + 1);
    stored_password.toCharArray(password, maxPasswordLength + 1);
}


void WiFiCredentials::saveToPersistentStorage(char *ssid, char *password) {
    preferences.putString("ssid", String(ssid).substring(0, maxSSIDLength));
    preferences.putString("password", String(password).substring(0, maxPasswordLength));
}