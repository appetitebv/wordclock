#include "Display.h"
#include "Clock.h"
#include "API.h"
#include "SunsetSunrise.h"
#include "Wifi.h"

Config config;
Display display;
Clock clock;
API api;
SunsetSunrise sunsetSunrise;
Wifi wifi;
  
void setup() {
  Serial.begin(115200);
  config.setup();
  wifi.setup();
  display.setup();
  clock.setup();
  api.setup();
  sunsetSunrise.setup();
  
  api.sync(&clock, &sunsetSunrise);
}

void loop() {
  api.loop(&clock, &sunsetSunrise);
  clock.loop(&display);
  sunsetSunrise.loop(&display, &clock);
}
