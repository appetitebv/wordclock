#include "Clock.h"

DS3231 RTC;

Clock::Clock() {
  Serial.println("Clock::Clock");
  this->setup();
}

void Clock::setup() {
  Serial.println("Clock::setup");
  
  // Start the I2C interface
  Wire.begin();

  // Set to 24h mode
  RTC.setClockMode(false);
}

void Clock::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
  RTC.setHour(hour);
  RTC.setMinute(minute);
  RTC.setSecond(second);
}

