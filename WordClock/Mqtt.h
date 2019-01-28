#ifndef Mqtt_h
#define Mqtt_h

#include <Arduino.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class Mqtt {
  public:
    Mqtt();
    void setup();
    void loop();
  private:
};
#endif
