#ifndef Wifi_h
#define Wifi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class Wifi {
  public:
    Wifi();
    void setup();
    void loop();
    void connect();
    bool connected();
  private:

};
#endif
