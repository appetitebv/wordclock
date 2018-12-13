#ifndef Display_h
#define Display_h

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

class Display {
  public:
    Display();
    void setup();
    void displayTemperature(uint8_t temperature);
    void displayTime(uint8_t hours, uint8_t minutes);
  private:
    void displayNumberAtPosition(uint8_t number, uint8_t position);
    void displayWordAt(uint8_t index);
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][9][4];
    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();
    static uint8_t wordMapping[23][6];
};
#endif
