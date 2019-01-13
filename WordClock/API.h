#ifndef API_h
#define API_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "Clock.h"
#include "SunsetSunrise.h"
#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class API {
  public:
    API();
    void setup();
    void loop(Clock *clock, SunsetSunrise *sunsetSunrise);
    void sync(Clock *clock, SunsetSunrise *sunsetSunrise);
  private:
    void connectToWifi();
    void updateFirmware(const char* host, const char* path);
    bool wifiConnected();
    Time parseTime(const char *string);
};
#endif
