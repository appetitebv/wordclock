#include "Display.h"
#include "Clock.h"
#include "API.h"
#include "SunsetSunrise.h"
#include "Wifi.h"
#include "WebServer.h"

Config config;
Display display;
Clock clock;
API api;
SunsetSunrise sunsetSunrise;
Wifi wifi;
WebServer webServer;
  
void setup() {
  Serial.begin(115200);
  config.setup();
  wifi.setup();
  webServer.setup(&wifi, &api);
  display.setup();
  clock.setup();
  sunsetSunrise.setup();
  api.setup(&clock, &sunsetSunrise);
  if (wifi.wifiConnected()) {
    api.sync();
  }
}

void loop() {
  wifi.loop();
  webServer.loop();
  api.loop();
  clock.loop(&display);
  sunsetSunrise.loop(&display, &clock);
}
