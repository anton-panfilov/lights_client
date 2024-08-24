class WiFiCredentials {
public:
    static const int maxSSIDLength = 32;
    static const int maxPasswordLength = 32;

    WiFiCredentials(int resetPinHoldOnBegin) : resetPinHoldOnBegin(resetPinHoldOnBegin) {
        init();
    }

    void begin();

    void loadFromPersistentStorage();

    char *getSSID() {
        return ssid;
    }

    char *getPassword() {
        return password;
    }

private:
    const int resetPinHoldOnBegin;
    char ssid[maxSSIDLength + 1] = "";
    char password[maxPasswordLength + 1] = "";

    /**
     * No use it so frequently, it can kill persistent memory on your board
     *
     * @param new_ssid
     * @param new_password
     */
    void saveToPersistentStorage(char *new_ssid, char *new_password);

    void init();

    void promptForCredentials(char* reason);
};

#include "Begin/ConsoleAvaliableOnStart.cpp"
#include "PromptForCredentials/USBConsole.cpp"

// different persistent storages implementations for get/set SSID/Password
#if defined(ESP32)
#include "PersistentStorage/ESP32.cpp"

#elif defined(ARDUINO_AVR) || defined(ARDUINO_AVR_UNO)
#include "PersistentStorage/AVR.cpp"

#elif defined(SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_NANO_33_IOT)
#include "PersistentStorage/SAMD.cpp"

#else
#error "Unsupported board selected!"
#endif
