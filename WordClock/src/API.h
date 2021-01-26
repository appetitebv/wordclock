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
  static Clock* _clock;
  static SunsetSunrise* _sunsetSunrise;
  public:
    API();
    void setup(Clock *clock, SunsetSunrise *sunsetSunrise);
    void loop();
    void sync();
    Time lastSync();
  private:
    void updateFirmware(const char* host, const char* path);
    Time parseTime(const char *string);
};
#endif
