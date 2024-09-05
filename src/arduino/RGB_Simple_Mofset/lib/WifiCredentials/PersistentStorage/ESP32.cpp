#include <Preferences.h>

Preferences preferences;

void WiFiCredentials::init() {}

void WiFiCredentials::loadFromPersistentStorage() {
    preferences.begin("wifiCredentials", true);

    String stored_ssid = preferences.getString("ssid", "");
    String stored_password = preferences.getString("password", "");

    stored_ssid.toCharArray(ssid, maxSSIDLength + 1);
    stored_password.toCharArray(password, maxPasswordLength + 1);
    preferences.end();
}


void WiFiCredentials::saveToPersistentStorage(char *ssid, char *password) {
    preferences.begin("wifiCredentials", false);
    preferences.putString("ssid", String(ssid).substring(0, maxSSIDLength));
    preferences.putString("password", String(password).substring(0, maxPasswordLength));
    preferences.end();
}