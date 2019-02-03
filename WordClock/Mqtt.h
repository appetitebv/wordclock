#ifndef Mqtt_h
#define Mqtt_h

#include <Arduino.h>

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
};
#endif
