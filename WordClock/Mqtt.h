#ifndef Mqtt_h
#define Mqtt_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Config.h"
#include "Display.h"

extern struct ConfigStorageStruct ClockConfig;

class Mqtt {
  Display* _display;
  public:
    Mqtt();
    void setup(Display *display);
    void loop();
  private:
    void setRgbState(String payload);
    void setBrightness(String payload);
    void publishLightState();
    void callback(char* p_topic, byte* p_payload, unsigned int p_length);
    void reconnect();
    
};
#endif
