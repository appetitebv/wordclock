#include <stdint.h>
#include "Display.h"
#include <Adafruit_NeoPixel.h>

#define PIXELS_PIN 2 // D4 on NodeMCU
#define PIXELS_COUNT 84 //  Total number of pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS_COUNT, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

// TODO: Default / saved color -> get this from EEPROM
uint32_t c = pixels.Color(127,127,127);

Display::Display() {
  pixels.begin();
  pixels.clear();
}

void Display::displayTemperature(uint8_t temperature) {
  // Only possible to display temperature of 2 digits.
  if (temperature < 100) {
    uint8_t lastDigit = temperature % 10;
    uint8_t firstDigit = (temperature-lastDigit)/10;
    Serial.print("First digit:");
    Serial.println(firstDigit);
    Serial.print("Last digit:");
    Serial.println(lastDigit);
    this->displayNumberAtPosition(firstDigit, 0);
    this->displayNumberAtPosition(lastDigit, 1);
  }
}

//  Number: number to display
//  Position: 0 = left, 1 = right
void Display::displayNumberAtPosition(uint8_t number, uint8_t position) {
  uint8_t cols = Display::numberMappingCols();
  uint8_t rows = Display::numberMappingRows();
  uint8_t shiftX = position*cols+position*1;
  Serial.print("ShiftX:");
  Serial.println(shiftX);
  pixels.clear();
  for (uint8_t y=0;y<rows;y++) {
    for (uint8_t x=0;x<cols;x++) {
      if (Display::numberMapping[number][y][x] == 1) {
        pixels.setPixelColor(Display::displayMapping[x+shiftX][y], c);
        Serial.print("X");
      } else {
        Serial.print(" ");
      }
    }
    Serial.println("");
  }
  pixels.show();
}

uint8_t Display::numberMappingRows() {
  uint8_t rows = sizeof Display::numberMapping[0] / sizeof Display::numberMapping[0][0];
  return rows;
}

uint8_t Display::numberMappingCols() {
  uint8_t cols = sizeof Display::numberMapping[0][0] / sizeof(uint8_t);
  return cols;
}

//  TODO: How to move this into PROGMEM?
uint8_t Display::displayMapping[9][9] = {
  { 0, 1, 2, 3, 4, 5, 6, 7, 8},
  {17,16,15,14,13,12,11,10, 9},
  {18,19,20,21,22,23,24,25,26},
  {35,34,33,32,31,30,29,28,27},
  {36,37,38,39,40,41,42,43,44},
  {53,52,51,50,49,48,47,46,45},
  {54,55,56,57,58,59,60,61,61},
  {70,69,68,67,66,65,64,63,62},
  {71,72,73,74,75,76,77,78,79}
};

//  TODO: How to move this into PROGMEM?
uint8_t Display::numberMapping[10][9][4] = {
  { //0
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },
    { //1
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,1,1,0}
  },
  { //2
    {1,1,1,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {1,1,1,1},
    {1,1,0,0},
    {1,1,0,0},
    {1,1,1,1},
    {1,1,1,1}
  },
  { // 3
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },
    { // 4
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1}
  },
  { // 5
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,0},
    {1,0,0,0},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },  { // 6
    {1,1,1,1},
    {1,1,1,1},
    {1,1,0,0},
    {1,1,0,0},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1}
  },  { // 7
    {1,1,1,1},
    {1,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1}
  },  { // 8
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {1,1,1,1},
  },
  { // 9
    {1,1,1,1},
    {1,1,1,1},
    {1,0,0,1},
    {1,0,0,1},
    {1,1,1,1},
    {0,0,1,1},
    {0,0,1,1},
    {1,1,1,1},
    {1,1,1,1}
  }
};

