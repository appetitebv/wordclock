#ifndef Wifi_h
#define Wifi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

#define WIFI_SSID "WordClock"

class Wifi {
  public:
    Wifi();
    void setup();
    void loop();
  private:
    void startAP();
    void connectToWifi();
    bool wifiConnected();
};
#endif
