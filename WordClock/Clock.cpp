#include "Clock.h"

DS3231 RTC;

// Bool if we're in a century year, don't need to be changed until year 2100.
bool Century=false; 
Time LastDisplayed = {
  0,0,0,0,0,0
};

Clock::Clock() {
  _timeInitiallySet = false;
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
    Serial.print("Display time:");
    Serial.print(CurrentTime.hour);
    Serial.print(":");
    Serial.println(CurrentTime.minute);
    display->displayTime(CurrentTime.hour, CurrentTime.minute);
  }

  LastDisplayed = CurrentTime;
}

void Clock::setTime(Time time) {
  _timeInitiallySet = true;
  RTC.setYear(time.year);
  RTC.setMonth(time.month);
  RTC.setDate(time.day);
  RTC.setHour(time.hour);
  RTC.setMinute(time.minute);
  RTC.setSecond(time.second);

  // Force display time
  LastDisplayed = {0,0,0,0,0,0};
}

uint16_t Clock::getYear() {
  return RTC.getYear();
}

uint8_t Clock::getMonth() {
  return RTC.getMonth(Century);
}

uint8_t Clock::getDate() {
  return RTC.getDate();
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
  if (_timeInitiallySet == false) {
    return LastDisplayed;
  }
  DateTime dateTime = RTClib::now();
  Time CurrentTime {
    dateTime.year(),
    dateTime.month(),
    dateTime.day(),
    dateTime.hour(),
    dateTime.minute(),
    dateTime.second()
  };
  return CurrentTime;
}

uint8_t Clock::getSecond() {
  return RTC.getSecond();
}

