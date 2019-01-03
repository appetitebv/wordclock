#include "Display.h"
#include "Clock.h"
#include "API.h"

Config config;
Display display;
Clock clock;
API api;
  
void setup() {
  Serial.begin(115200);
  config.setup();
  display.setup();
  clock.setup();
  api.setup();
  api.sync(&clock);
}

void loop() {
  api.loop();
  clock.loop(&display);
}

