#include "SunsetSunrise.h"

Time Sunset {
  17,00,0
};

Time Sunrise {
  7,0,0
};

Time LastChange {
  0,0,0
};

SunsetSunrise::SunsetSunrise() {
}

void SunsetSunrise::setup() {
  Serial.println("SunsetSunrise::setup");
}

void SunsetSunrise::loop(Display *display, Clock *clock) {
  Time currentTime = clock->getTime();

  // First check if we need to set brightness initially, on startup
  if (LastChange.hour == 0 && LastChange.minute == 0) {
    if ((currentTime.hour >= Sunset.hour && currentTime.minute >= Sunset.minute) || (currentTime.hour <= Sunrise.hour && currentTime.minute < Sunrise.minute)) {
      this->setNight(display);
    } else {
      this->setDay(display);
    }
    LastChange = currentTime;
  } else if ((currentTime.hour != LastChange.hour) && (currentTime.minute != LastChange.minute)) {
    if (currentTime.hour == Sunrise.hour && currentTime.minute == Sunrise.minute) {
      this->setDay(display);
      LastChange = currentTime;
    } else if (currentTime.hour == Sunset.hour && currentTime.minute == Sunset.minute) {
      this->setNight(display);
      LastChange = currentTime;
    }
  }
}

void SunsetSunrise::set(Time sunrise, Time sunset) {
  Sunrise = sunrise;
  Sunset = sunset;
}

void SunsetSunrise::setDay(Display *display) {
  display->setBrightness(ClockConfig.clockBrightnessDay);
}

void SunsetSunrise::setNight(Display *display) {
  display->setBrightness(ClockConfig.clockBrightnessNight);
}

