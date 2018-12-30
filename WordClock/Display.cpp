#include <stdint.h>
#include "Display.h"
#include <Adafruit_NeoPixel.h>

#define PIXELS_PIN 2 // D4 on NodeMCU
#define PIXELS_COUNT 85 //  Total number of pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS_COUNT, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

Display::Display() {
}

void Display::setup() {
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

        uint8_t pixel = pgm_read_byte(&Display::displayMapping[x+shiftX][y]);
        
        pixels.setPixelColor(pixel, ClockConfig.clockColor);
        Serial.print("X");
      } else {
        Serial.print(" ");
      }
    }
    Serial.println("");
  }
  pixels.show();
}


/**
 * Reading a word from the PROGMEM
 */
void Display::displayWordAt(uint8_t index) {

  //size is always 5, hardcodede for now
  for (uint8_t i=0; i < 5; i++) {
    uint8_t pixel = pgm_read_byte(&Display::wordMapping[index][i]);
    if (pixel != 99) {
      pixels.setPixelColor(pixel, ClockConfig.clockColor);
    }
  }
}

/**
 * Display the time on the display
 */
void Display::displayTime(uint8_t hours, uint8_t minutes) {
  pixels.clear();

  // 10:20 => tien voor half ELF
  if (minutes >= 20) {
    hours = hours+1;
  }

  // 12 hours clock
  hours = hours%12;

  // Hours
  if (hours == 0) {
    this->displayWordAt(11);
  } else if (hours >= 1 && hours <= 11) {
    this->displayWordAt(hours-1);
  }

  // 
  if (minutes >= 0 && minutes < 5) {
    this->displayWordAt(18); // Uur
  } else if (minutes >= 5 && minutes < 10) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(16); // Over
  } else if (minutes >= 10 && minutes < 15) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(16); // Over
  } else if (minutes >= 15 && minutes < 20) {
    this->displayWordAt(13); // Kwart
    this->displayWordAt(16); // Over
  } else if (minutes >= 20 && minutes < 25) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(15); // Voor
    this->displayWordAt(17); // Half
  } else if (minutes >= 25 && minutes < 30) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(15); // Voor
    this->displayWordAt(17); // Half
  } else if (minutes >= 30 && minutes < 35) {
    this->displayWordAt(17); // Half
  } else if (minutes >= 35 && minutes < 40) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(16); // Over
    this->displayWordAt(17); // Half
  } else if (minutes >= 40 && minutes < 45) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(16); // Over
    this->displayWordAt(17); // Half
  } else if (minutes >= 45 && minutes < 50) {
    this->displayWordAt(13); // Kwart
    this->displayWordAt(15); // Voor
  } else if (minutes >= 50 && minutes < 55) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(15); // Voor
  } else if (minutes >= 55 && minutes < 60) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(15); // Voor
  }

  switch (minutes%5) {
    case 1: 
      this->displayWordAt(19); // 1
      break;
    case 2:
      this->displayWordAt(20); // 2
      break;
    case 3:
      this->displayWordAt(21); // 3
      break;
    case 4:
      this->displayWordAt(22); // 4
      break;
    default:
      break;
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

/**
 * Function that shows a number from PROGMEM on the display
 * - Used for displaying temperature
 */
struct NUMBER_IN_DISPLAY Display::NUMBER_load_progmem(uint8_t number) {

  struct NUMBER_IN_DISPLAY n;
  
  // number 1-10
  // r = rows
  for (unsigned char r=0; r<N_HEIGHT; r++) {
    for (unsigned char i=0; i<N_WIDTH; i++){
      n.matrix[r][i] = pgm_read_byte(&Display::numberMapping[number][r][i]);
    } 
  }
  return n;
}

uint8_t Display::wordMapping[23][6] PROGMEM = {
  // Enter 99 to ignore
  
  // Hours
  {74,75,76,99,99,99}, // Een
  {72,73,74,75,99,99}, // Twee
  {36,37,38,39,99,99}, // Drie
  {71,70,69,68,99,99}, // Vier
  {66,65,64,63,99,99}, // Vijf
  {29,28,28,99,99,99}, // Zes
  {54,55,56,57,58,99}, // Zeven
  {41,42,43,44,99,99}, // Acht
  {50,49,48,47,46,99}, // Negen
  {53,52,51,50,99,99}, // Tien
  {60,61,62,99,99,99}, // Elf
  {35,34,33,32,31,30}, // Twaalf

  // Words
  { 0, 1, 2, 3,99,99}, // Vijf
  { 4, 5, 6, 7, 8,99}, // Kwart
  {17,16,15,14,99,99}, // Tien
  {12,11,10, 9,99,99}, // Voor
  {18,19,20,21,99,99}, // Over
  {23,24,25,26,99,99}, // Half
  {78,79,80,99,99,99}, // Uur

  // Minutes
  {84,99,99,99,99,99}, // 1
  {84,83,99,99,99,99}, // 2
  {84,83,82,99,99,99}, // 3
  {84,83,82,81,99,99}  // 4
};

uint8_t Display::displayMapping[9][9] PROGMEM = {
  { 0, 1, 2, 3, 4, 5, 6, 7, 8},
  {17,16,15,14,13,12,11,10, 9},
  {18,19,20,21,22,23,24,25,26},
  {35,34,33,32,31,30,29,28,27},
  {36,37,38,39,40,41,42,43,44},
  {53,52,51,50,49,48,47,46,45},
  {54,55,56,57,58,59,60,61,62},
  {71,70,69,68,67,66,65,64,63},
  {72,73,74,75,76,77,78,79,80}
};


uint8_t Display::numberMapping[10][N_HEIGHT][N_WIDTH] PROGMEM = {
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
    {1,0,0,0},
    {1,0,0,0},
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
