#include "Display.h"
#include "Clock.h"
#include "API.h"
#include "SunsetSunrise.h"
#include "Wifi.h"
#include "WebServer.h"
#include "Mqtt.h"
#include "Config.h"

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
  
  display.setup();
  display.startLoading();
  
  config.setup();
  wifi.setup();
  webServer.setup(&wifi, &api, &sunsetSunrise, &display, &mqtt, &clock);
  clock.setup();
  sunsetSunrise.setup(&mqtt);
  api.setup(&clock, &sunsetSunrise);
  if (ClockConfig.mqttEnabled) {
    mqtt.setup(&display);
  }
}

void loop() {
  wifi.loop();
  display.loop();
  if (wifi.wifiConnected()) {
    display.stopLoading();
    api.loop();
    if (ClockConfig.mqttEnabled) {
      mqtt.loop();
    }
  }
  webServer.loop();
  clock.loop(&display);
  sunsetSunrise.loop(&display, &clock);
}
