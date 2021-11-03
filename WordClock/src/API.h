#ifndef API_h
#define API_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "Clock.h"
#include "SunsetSunrise.h"
#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class API {
  public:
    API(Clock& clock, SunsetSunrise& sunsetSunrise, WiFiClient& wifiClient) : _clock(clock), _sunsetSunrise(sunsetSunrise), _wifiClient(wifiClient) {} ;
    void setup();
    void loop();
    void sync();
    Time lastSync();
  private:
    Clock& _clock;
    SunsetSunrise& _sunsetSunrise;
    WiFiClient& _wifiClient;
    void updateFirmware(const char* host, const char* path);
    Time parseTime(const char *string);
};
#endif
