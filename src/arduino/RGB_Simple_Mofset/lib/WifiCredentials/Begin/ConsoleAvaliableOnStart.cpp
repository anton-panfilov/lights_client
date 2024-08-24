#define HOOPS_COUNT 10
#define HOOP_DELAY 100

static bool isChangeWifiCredentialsRead() {
    bool result = false;
    bool first = true;
    while (true) {
        if (Serial.available()) {
            char incomingByte = Serial.read();
            if (incomingByte == '\n') {
                break;
            }
            if (first && incomingByte == 'y') {
                result = true;
            }
        }
    }
    return result;
}

void WiFiCredentials::begin() {
    bool has_console = false;
    int current_hoop = 0;

    while(current_hoop < HOOPS_COUNT){
        if(Serial){
            has_console = true;
            break;
        }
        delay(HOOP_DELAY);
        current_hoop++;
    }

    int resetState = digitalRead(resetPinHoldOnBegin);
    loadFromPersistentStorage();

    if (strlen(ssid) == 0 ) {
        promptForCredentials("ssid not set");
    } else if (has_console) {
        while (!Serial) {}
        Serial.print("current ssid is:");
        Serial.println(ssid);
        Serial.println("are you want to change current wi-fi credentials (y/n):");
        if(isChangeWifiCredentialsRead()){
            promptForCredentials("Serial found and you want to change the ");
        } else {
            Serial.println("continue with current wi-fi credentials");
        }
    }
}
