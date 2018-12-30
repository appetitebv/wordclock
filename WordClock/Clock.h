#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include <DS3231.h> // https://github.com/NorthernWidget/DS3231
#include <Wire.h> // Connected via I2C-bus on pins D1 (GPIO5/SCL) and D2 (GPIO4/SDA)

class Clock {
  public:
    Clock();
    void setup();
    void setTime(uint8_t hour, uint8_t minute, uint8_t second);
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
};
#endif
