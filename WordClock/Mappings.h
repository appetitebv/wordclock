#ifndef Mappings_h
#define Mappings_h

#include <Arduino.h>

class Mappings {
  public:
    static uint8_t wordMapping[23][6];
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][9][4];
    static uint8_t characterMapping[26][9][9];

    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();
};
#endif
