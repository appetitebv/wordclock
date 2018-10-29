#include "Display.h"

void setup() {
  Serial.begin(115200);
  
  Serial.println("Create display");
  Display display;
  
  Serial.println("Display temperatures");
  display.displayTemperature(01);
  display.displayTemperature(23);
  display.displayTemperature(45);
  display.displayTemperature(67);
  display.displayTemperature(89);
  display.displayTemperature(10);
}

void loop() {
  // put your main code here, to run repeatedly:

}
