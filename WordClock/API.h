#ifndef API_h
#define API_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "Wifi.h"

#include "Clock.h"
#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class API {
  public:
    API();
    void setup();
    void loop();
    void sync(Clock *clock);
};
#endif
