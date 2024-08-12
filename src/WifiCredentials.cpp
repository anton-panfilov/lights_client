#include <EEPROM.h>

class WiFiCredentials {
  public:
    WiFiCredentials(int resetPin) : pinReset(resetPin) {
      pinMode(pinReset, INPUT);
    }

    void begin() {
      Serial.begin(9600);
      while (!Serial) {}
      int resetState = digitalRead(pinReset);

      readFromEEPROM(ssidAddress, ssid, maxSSIDLength);
      readFromEEPROM(passwordAddress, password, maxPasswordLength);

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
    static const int ssidAddress = 0;
    static const int passwordAddress = maxSSIDLength + 1;
    char ssid[maxSSIDLength + 1];
    char password[maxPasswordLength + 1];

    void promptForCredentials() {
      Serial.println("Enter WiFi SSID:");
      readInput(ssid, maxSSIDLength);
      saveToEEPROM(ssidAddress, ssid, maxSSIDLength);

      Serial.println("\nEnter WiFi password:");
      readInput(password, maxPasswordLength);
      saveToEEPROM(passwordAddress, password, maxPasswordLength);
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

    void saveToEEPROM(int startAddress, const char* data, int maxLength) {
      for (int i = 0; i < maxLength; i++) {
        if (i < strlen(data)) {
          EEPROM.write(startAddress + i, data[i]);
        } else {
          EEPROM.write(startAddress + i, 0);
        }
      }
    }

    void readFromEEPROM(int startAddress, char* buffer, int maxLength) {
      for (int i = 0; i < maxLength; i++) {
        buffer[i] = EEPROM.read(startAddress + i);
      }
      buffer[maxLength] = '\0';
    }
};