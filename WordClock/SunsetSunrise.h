#ifndef SunsetSunrise_h
#define SunsetSunrise_h

#include <Arduino.h>
#include "Config.h"
#include "Clock.h"
#include "Display.h"
#include "Mqtt.h"

extern struct ConfigStorageStruct ClockConfig;

class SunsetSunrise {
  static Mqtt* _mqtt;
  public:
    SunsetSunrise();
    void setup(Mqtt *mqtt);
    void set(Time sunrise, Time sunset);
    void loop(Display *display, Clock *clock);
    Time sunrise();
    Time sunset();
  private:
    void setDay(Display *display);
    void setNight(Display *display);
};
#endif
