#include "Display.h"
#include "Clock.h"

Config config;
Display display;
Clock clock;
  
void setup() {
  Serial.begin(115200);
  config.setup();
  display.setup();
  clock.setup();
}

void loop() {
  // put your main code here, to run repeatedly:

//  Serial.println("Display temperatures");
//  display.displayTemperature(01);
//  delay(500);
//  display.displayTemperature(23);
//  delay(500);
//  display.displayTemperature(45);
//  delay(500);
//  display.displayTemperature(67);
//  delay(500);
//  display.displayTemperature(89);
//  delay(500);
//  display.displayTemperature(10);
//  delay(500);
  demoClock();
}

void demoClock() {
  for(int hours=0;hours<24;hours++) {
    for(int minutes=0;minutes<60;minutes++) {
      display.displayTime(hours,minutes);
      delay(500);
    }
  }
}

