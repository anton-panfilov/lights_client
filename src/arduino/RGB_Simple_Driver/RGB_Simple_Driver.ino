#include "WiFiS3.h"
#include <FastLED.h>
#include "RGBClientAPI.cpp"
#include "WifiCredentials.cpp"

/////////////////////////////////////////////////////////////////////
// settings

#define DATA_PIN 3
#define CLOCK_PIN 13
#define RESET_WIFI_CREDENTIALS_PIN 2

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

CRGB leds[1];

int wifi_status = WL_IDLE_STATUS;
int wifi_errors_count = 0;
int server_errors_count = 0;


//////////////////////////////////////////////////////////////////
// functions

void ledTurnOff() {
  leds[0] = CRGB::Black;
  FastLED.show();
  Serial.println("turn off the led strip");
}


//////////////////////////////////////////////////////////////////
// setup

void setup() {
  Serial.begin(9600);
  wifiCredentials.begin();
  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, 1);
  ledTurnOff();
}

//////////////////////////////////////////////////////////////////
// loop

void loop() {
  if(wifi_status != WL_CONNECTED){
    wifi_errors_count = 0;
    while (wifi_status != WL_CONNECTED) {
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(wifiCredentials.getSSID());
      wifi_status = WiFi.begin(wifiCredentials.getSSID(), wifiCredentials.getPassword());
      if(wifi_status != WL_CONNECTED){
        wifi_errors_count++;
        if(wifi_errors_count > TURN_OFF_AFTER_X_WIFI_ERRORS){
          ledTurnOff();
        }
      }
    }
    Serial.println("Connected to Wi-Fi!");
  }
  if(rgbClient.getColorFromServer()){
    Serial.print("color: ");
    Serial.println(rgbClient.color.hex());

    server_errors_count = 0;
    leds[0] = CRGB(rgbClient.color.R, rgbClient.color.G, rgbClient.color.B);
    FastLED.show();
  } else {
    server_errors_count++;
    Serial.println("can't to get color from server");
    if(server_errors_count > TURN_OFF_AFTER_X_SERVER_ERRORS){
      ledTurnOff();
    }
  }
}
