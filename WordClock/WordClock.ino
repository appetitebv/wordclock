#include "Display.h"

Display display;
  
void setup() {
  Serial.begin(115200);
  display.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Reading from PROGMEM");
  struct NUMBER_IN_DISPLAY number;
  number = display.NUMBER_load_progmem(8);
  print_number(number);

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

static void print_number(struct NUMBER_IN_DISPLAY number) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 4; j++)
    Serial.print(number.matrix[i][j]);
    Serial.print("\n");
  }
}
