#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include <DS3231.h> // https://github.com/NorthernWidget/DS3231
#include <Wire.h> // Connected via I2C

//#include "Config.h"
//extern struct ConfigStorageStruct ClockConfig;

class Clock {
  public:
    Clock();
    void setup();
    void setTime(uint8_t hour, uint8_t minute, uint8_t second);
};
#endif
