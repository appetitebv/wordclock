#include "Clock.h"

DS3231 RTC;

Time LastDisplayed = {
  0,0
};

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

void Clock::loop(Display *display) {

  Time CurrentTime = this->getTime();
  
  if (CurrentTime.hour != LastDisplayed.hour || CurrentTime.minute != LastDisplayed.minute) {
    display->displayTime(CurrentTime.hour, CurrentTime.minute);
  }

  LastDisplayed = CurrentTime;
}

void Clock::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
  RTC.setHour(hour);
  RTC.setMinute(minute);
  RTC.setSecond(second);
}

uint8_t Clock::getHour() {
  bool h12;     // false = 24h, true = 12h
  bool PM_time;
  return RTC.getHour(h12, PM_time);
}

uint8_t Clock::getMinute() {
  return RTC.getMinute();
}

Time Clock::getTime() {
  Time CurrentTime {
    this->getHour(),
    this->getMinute()
  };
  return CurrentTime;
}

uint8_t Clock::getSecond() {
  return RTC.getSecond();
}

