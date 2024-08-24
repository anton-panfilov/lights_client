#include <FlashStorage.h>

typedef struct {
    char ssid[WiFiCredentials::maxSSIDLength + 1];
    char password[WiFiCredentials::maxPasswordLength + 1];
} CredentialsStruct;

FlashStorage(
        wifiCredentialsStore,
        CredentialsStruct
);

static CredentialsStruct credentials;

void WiFiCredentials::init() {}

void WiFiCredentials::loadFromPersistentStorage() {
    credentials = wifiCredentialsStore.read();

    strncpy(ssid, credentials.ssid, maxSSIDLength);
    strncpy(password, credentials.password, maxSSIDLength);
}

void WiFiCredentials::saveToPersistentStorage(char *new_ssid, char *new_password) {
    strncpy(ssid, new_ssid, maxSSIDLength);
    strncpy(password, new_password, maxPasswordLength);

    strncpy(credentials.ssid, ssid, maxSSIDLength);
    strncpy(credentials.password, password, maxPasswordLength);

    wifiCredentialsStore.write(credentials);
}