#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#if defined(ARDUINO_AVR_UNO)
  #include <WiFiS3.h> // WiFi library for Arduino Uno WiFi
#elif defined(ESP32)
  #include <WiFi.h> // WiFi library for ESP32
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
  #include <WiFiNINA.h>
#else
  #error "Unsupported board selected!"
#endif

class RGBColor {
  public:
    int R, G, B;
    RGBColor(int R = 0, int G = 0, int B = 0) :R(R), G(G), B(B) {}
    char* hex() {
        snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", R, G, B);
        return hexColor;
    }
  private:
    char hexColor[8];
};

class RGBClientAPI {
  public:
    RGBColor color;

    RGBClientAPI(
      WiFiClient& wifi, 
      const char* serverName, 
      int port
    ): 
      wifi(wifi), 
      port(port)
    {
      strncpy(this->serverName, serverName, sizeof(this->serverName) - 1);
      this->serverName[sizeof(this->serverName) - 1] = '\0';

      client = new HttpClient(wifi, this->serverName, this->port);
    }

    bool getColorFromServer() {
      client->get("/get-color");

      int statusCode = client->responseStatusCode();
      String response = client->responseBody();

      if(statusCode == 200){
        json_error = deserializeJson(json, response);
        if (!json_error) {
          color.R = json["R"].as<int>();
          color.G = json["G"].as<int>();
          color.B = json["B"].as<int>();
          return true;
        }
      }
      return false;
    }

    ~RGBClientAPI() {
      delete client;
    }
  
  private: 
    HttpClient* client;
    WiFiClient wifi;
    char serverName[256];  // Adjust size as needed
    int port;
    JsonDocument json;
    DeserializationError json_error;  
};