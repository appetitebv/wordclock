#include "Display.h"

void setup() {
  Serial.begin(115200);
  
  Serial.println("Create display");
  Display display;
//  
  Serial.println("Display temperatures");
//  display.displayTemperature(01);
//  display.displayTemperature(23);
//  display.displayTemperature(45);
//  display.displayTemperature(67);
//  display.displayTemperature(89);
//  display.displayTemperature(10);

  Serial.println("Reading from PROGMEM");
  struct NUMBER_IN_DISPLAY number;
  number = display.NUMBER_load_progmem(8);

  print_number(number);

  Serial.println("Done setup");
}

void loop() {
  // put your main code here, to run repeatedly:

}

static void print_number(struct NUMBER_IN_DISPLAY number)
{
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 4; j++)
      Serial.print(number.matrix[i][j]);
    Serial.print("\n");
  }
}
