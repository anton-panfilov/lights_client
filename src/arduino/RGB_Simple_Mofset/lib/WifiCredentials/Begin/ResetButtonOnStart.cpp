void WiFiCredentials::begin() {
    int resetState = digitalRead(resetPinHoldOnBegin);
    loadFromPersistentStorage();

    if (strlen(ssid) == 0 ) {
        promptForCredentials("ssid not set");
    }

    if (resetState == HIGH) {
        promptForCredentials("reset button is pressed");
    }
}
