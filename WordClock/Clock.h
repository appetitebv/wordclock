#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include "Display.h"
#include <DS3231.h> // https://github.com/NorthernWidget/DS3231
#include <Wire.h> // Connected via I2C-bus on pins D1 (GPIO5/SCL) and D2 (GPIO4/SDA)

struct Time {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

class Clock {
  public:
    Clock();
    void setup();
    void loop(Display *display);
    void setTime(Time time);
    Time getTime();
  private:
    uint8_t getYear();
    uint8_t getMonth();
    uint8_t getDate();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
};
#endif
