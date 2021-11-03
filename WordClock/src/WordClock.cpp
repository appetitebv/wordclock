#include "Display.h"
#include "Clock.h"
#include "API.h"
#include "SunsetSunrise.h"
#include "Wifi.h"
#include "WebServer.h"
#include "Mqtt.h"
#include "Config.h"

Config config;
WiFiClient wifiClient;
Display display;
Clock wordClock;
SunsetSunrise sunsetSunrise;
API api(wordClock, sunsetSunrise, wifiClient);
Wifi wifi;
WebServer webServer;
Mqtt mqtt(display, wifiClient);
  
void setup() {
  Serial.begin(115200);
  
  display.setup();
  display.startLoading();
  
  config.setup();
  wifi.setup();
  webServer.setup(&wifi, &api, &sunsetSunrise, &display, &mqtt, &wordClock);
  wordClock.setup();
  sunsetSunrise.setup(&mqtt);
  api.setup();
  if (ClockConfig.mqttEnabled) {
    mqtt.setup();
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
  wordClock.loop(&display);
  sunsetSunrise.loop(&display, &wordClock);
}
