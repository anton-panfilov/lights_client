#if defined(ARDUINO_AVR_UNO)
#include <WiFiS3.h> // WiFi library for Arduino Uno WiFi
#elif defined(ESP32)
#include <WiFi.h> // WiFi library for ESP32
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#include <WiFiNINA.h>
#else
#error "Unsupported board selected!"
#endif

#include "RGBClientAPI.cpp"
#include "WifiCredentials.cpp"

/////////////////////////////////////////////////////////////////////
// settings

#define COLOR_R_PIN A7
#define COLOR_G_PIN A4
#define COLOR_B_PIN A0
#define RESET_WIFI_CREDENTIALS_PIN A6

#define SERVER_HOST "lights-control-server.leaptheorytech.com"
#define SERVER_PORT 80

#define TURN_OFF_AFTER_X_WIFI_ERRORS 5
#define TURN_OFF_AFTER_X_SERVER_ERRORS 10


//////////////////////////////////////////////////////////////////
// allocate memory

WiFiCredentials wifiCredentials(RESET_WIFI_CREDENTIALS_PIN);
WiFiClient wifi;

RGBClientAPI rgbClient(
  wifi, 
  SERVER_HOST, 
  SERVER_PORT
);

int wifi_status = WL_IDLE_STATUS;
int wifi_errors_count = 0;
int server_errors_count = 0;
RGBColor defaultColor(0, 0, 0);


//////////////////////////////////////////////////////////////////
// functions

void setColor(RGBColor& color) {
  analogWrite(COLOR_R_PIN, color.R);
  analogWrite(COLOR_G_PIN, color.G);
  analogWrite(COLOR_B_PIN, color.B);

  #if defined(ESP32)
    analogWrite(LED_RED, 255-color.R);
    analogWrite(LED_GREEN, 255-color.G);
    analogWrite(LED_BLUE, 255-color.B);
  #endif
}

void ledTurnOff() {
  setColor(defaultColor);
}


//////////////////////////////////////////////////////////////////
// setup

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  #if defined(ESP32)
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
  #endif

  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, 1);
  ledTurnOff();
}

//////////////////////////////////////////////////////////////////
// loop

void loop() {
  if(wifi_status != WL_CONNECTED){
    digitalWrite(LED_BUILTIN, 1);
    Serial.println("wifiCredentials.begin()");
    wifiCredentials.begin();
    wifi_errors_count = 0;
    while (wifi_status != WL_CONNECTED) {
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(wifiCredentials.getSSID());
      
      #if defined(ARDUINO_AVR_UNO)
        wifi_status = WiFi.begin(wifiCredentials.getSSID(), wifiCredentials.getPassword());
      #elif defined(ARDUINO_SAMD_NANO_33_IOT)
        wifi_status = WiFi.begin(wifiCredentials.getSSID(), wifiCredentials.getPassword());
      #elif defined(ESP32)
        WiFi.begin(wifiCredentials.getSSID(), wifiCredentials.getPassword());
        wifi_status = WiFi.waitForConnectResult();
      #endif

      if(wifi_status != WL_CONNECTED){
        wifi_errors_count++;
        if(wifi_errors_count > TURN_OFF_AFTER_X_WIFI_ERRORS){
          ledTurnOff();
        }
      }
    }
    Serial.println("Connected to Wi-Fi!");
  }
  digitalWrite(LED_BUILTIN, 0);
  if(rgbClient.getColorFromServer()){
    Serial.print("color: ");
    Serial.println(rgbClient.color.hex());

    server_errors_count = 0;
    setColor(rgbClient.color);
  } else {
    server_errors_count++;
    Serial.println("can't to get color from server");
    if(server_errors_count > TURN_OFF_AFTER_X_SERVER_ERRORS){
      ledTurnOff();
    }
  }
}
