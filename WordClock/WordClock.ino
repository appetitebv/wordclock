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
  // put your main code here, to run repeatedly:
  Serial.println("Reading from PROGMEM");
  struct NUMBER_IN_DISPLAY number;
  number = display.NUMBER_load_progmem(8);
  print_number(number);

  api.loop();
  clock.loop(&display);
}

static void print_number(struct NUMBER_IN_DISPLAY number) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 4; j++)
    Serial.print(number.matrix[i][j]);
    Serial.print("\n");
  }
}
