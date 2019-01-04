#ifndef API_h
#define API_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "Clock.h"
#include "SunsetSunrise.h"
#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class API {
  public:
    API();
    void setup();
    void loop();
    void sync(Clock *clock, SunsetSunrise *sunsetSunrise);
  private:
    void connectToWifi();
    bool wifiConnected();
    Time parseTime(const char *string);
};
#endif
