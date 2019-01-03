#ifndef Display_h
#define Display_h

#include <stdint.h>
#include <Arduino.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

//Number height and width of pixels
#define N_HEIGHT 9
#define N_WIDTH 4

#define PIXELS_PIN 2 // D4 on NodeMCU
#define PIXELS_COUNT 85 //  Total number of pixels

#include <Adafruit_NeoPixel.h>

class Display {
  
  public:
    Display();
    void setup();
    void displayTemperature(uint8_t temperature);
    void displayTime(uint8_t hours, uint8_t minutes);
    struct NUMBER_IN_DISPLAY NUMBER_load_progmem(uint8_t number);
  private:
    void displayNumberAtPosition(uint8_t number, uint8_t position);
    void displayWordAt(uint8_t index);
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][N_HEIGHT][N_WIDTH];
    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();
    static uint8_t wordMapping[23][6];
};

struct NUMBER_IN_DISPLAY {
    int matrix[N_HEIGHT][N_WIDTH];
};
#endif
