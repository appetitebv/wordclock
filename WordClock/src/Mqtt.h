#ifndef Mqtt_h
#define Mqtt_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Config.h"
#include "Display.h"

extern struct ConfigStorageStruct ClockConfig;

class Mqtt {
  public:
    Mqtt(Display& display, WiFiClient& wifiClient) : _pubSubClient(PubSubClient(wifiClient)), _display(display) {};
    void setup();
    void loop();
    void publishBrightness(uint8_t brightness);
    void publishColor(uint32_t color);
  private:
    PubSubClient _pubSubClient;
    Display& _display;
    void setRgbState(String payload);
    void setBrightness(String payload);
    void publishLightState();
    void callback(char* p_topic, byte* p_payload, unsigned int p_length);
    void reconnect();
    
};
#endif
