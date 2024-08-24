static void readInput(char *buffer, int maxLength) {
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

void WiFiCredentials::promptForCredentials(char* reason) {
    while (!Serial) {}

    Serial.print("Credentials change reason: ");
    Serial.println(reason);

    Serial.println("Enter WiFi SSID:");
    readInput(ssid, maxSSIDLength);

    Serial.println("Enter WiFi password:");
    readInput(password, maxPasswordLength);

    Serial.println("Credentials saved!");
    saveToPersistentStorage(ssid, password);
}
