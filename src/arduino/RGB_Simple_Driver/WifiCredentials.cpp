#ifdef ESP32
  #include <Preferences.h>
  #define USE_PREFERENCES
#else
  #include <EEPROM.h>
#endif

class WiFiCredentials {
  public:
    WiFiCredentials(int resetPin) : pinReset(resetPin) {
      pinMode(pinReset, INPUT);
    }

    void begin() {
      int resetState = digitalRead(pinReset);

      #ifdef USE_PREFERENCES
        preferences.begin("wifiCreds", false);
        readFromPreferences("ssid", ssid, maxSSIDLength);
        readFromPreferences("password", password, maxPasswordLength);
      #else
        EEPROM.begin(512); // Initialize EEPROM with a size of 512 bytes
        readFromEEPROM(ssidAddress, ssid, maxSSIDLength);
        readFromEEPROM(passwordAddress, password, maxPasswordLength);
      #endif

      if (strlen(ssid) == 0 || strlen(password) == 0 || resetState == HIGH) {
        promptForCredentials();
      }
    }

    const char* getSSID() const {
      return ssid;
    }

    const char* getPassword() const {
      return password;
    }

  private:
    const int pinReset;
    static const int maxSSIDLength = 32;
    static const int maxPasswordLength = 32;
    char ssid[maxSSIDLength + 1];
    char password[maxPasswordLength + 1];

    #ifdef USE_PREFERENCES
      Preferences preferences;
    #else
      static const int ssidAddress = 0;
      static const int passwordAddress = maxSSIDLength + 1;
    #endif

    void promptForCredentials() {
      while (!Serial) {}
      Serial.println("Enter WiFi SSID:");
      readInput(ssid, maxSSIDLength);
      #ifdef USE_PREFERENCES
        saveToPreferences("ssid", ssid, maxSSIDLength);
      #else
        saveToEEPROM(ssidAddress, ssid, maxSSIDLength);
      #endif

      Serial.println("\nEnter WiFi password:");
      readInput(password, maxPasswordLength);
      #ifdef USE_PREFERENCES
        saveToPreferences("password", password, maxSSIDLength);
      #else
        saveToEEPROM(passwordAddress, password, maxPasswordLength);
      #endif
    }

    void readInput(char* buffer, int maxLength) {
      int index = 0;
      while (true) {
        if (Serial.available()) {
          char incomingByte = Serial.read();
          if (incomingByte == '\n') {
            break;
          }
          if (index < maxLength) {
            buffer[index] = incomingByte;
            index++;
          }
        }
      }
      buffer[index] = '\0'; // Null-terminate the string
    }

#ifdef USE_PREFERENCES
    void saveToPreferences(const char* key, const char* data, int maxLength) {
      preferences.putString(key, String(data).substring(0, maxLength));
    }

    void readFromPreferences(const char* key, char* buffer, int maxLength) {
      String value = preferences.getString(key, "");
      value.toCharArray(buffer, maxLength + 1);
    }
#else
    void saveToEEPROM(int startAddress, const char* data, int maxLength) {
      for (int i = 0; i < maxLength; i++) {
        if (i < strlen(data)) {
          EEPROM.write(startAddress + i, data[i]);
        } else {
          EEPROM.write(startAddress + i, 0);
        }
      }
      EEPROM.commit(); // Commit changes to EEPROM
    }

    void readFromEEPROM(int startAddress, char* buffer, int maxLength) {
      for (int i = 0; i < maxLength; i++) {
        buffer[i] = EEPROM.read(startAddress + i);
      }
      buffer[maxLength] = '\0';
    }
#endif
};