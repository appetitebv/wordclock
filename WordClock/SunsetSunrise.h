#ifndef SunsetSunrise_h
#define SunsetSunrise_h

#include <Arduino.h>
#include "Config.h"
#include "Clock.h"
#include "Display.h"

extern struct ConfigStorageStruct ClockConfig;

class SunsetSunrise {
  public:
    SunsetSunrise();
    void setup();
    void set(Time sunrise, Time sunset);
    void loop(Display *display, Clock *clock);
  private:
    void setDay(Display *display);
    void setNight(Display *display);
};
#endif
