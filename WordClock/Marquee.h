#ifndef Marquee_h
#define Marquee_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Config.h"
#include "Display.h"

extern struct ConfigStorageStruct ClockConfig;

class Marquee {
  Display* _display;
  public:
    Marquee();
    void setup(Display *display);
    void loop();
  private:
    
};
#endif
