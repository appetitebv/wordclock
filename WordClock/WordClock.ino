#include "Display.h"
#include "Clock.h"
#include "API.h"
#include "SunsetSunrise.h"
#include "Wifi.h"
#include "WebServer.h"
#include "Mqtt.h"

Config config;
Display display;
Clock clock;
API api;
SunsetSunrise sunsetSunrise;
Wifi wifi;
WebServer webServer;
Mqtt mqtt;
  
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

  // TODO: only if enabled in settings
  mqtt.setup();

}

void loop() {
  wifi.loop();
  webServer.loop();
  api.loop();
  clock.loop(&display);
  sunsetSunrise.loop(&display, &clock);
  mqtt.loop();
}
