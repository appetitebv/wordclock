#ifndef Display_h
#define Display_h

#include <stdint.h>
#include <Arduino.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

#define PIXELS_PIN 2 // D4 on NodeMCU
#define PIXELS_COUNT 85 //  Total number of pixels

#include <Adafruit_NeoPixel.h>

class Display {
  
  public:
    Display();
    void setup();
    void loop();
    void displayTemperature(uint8_t temperature);
    void displayTime(uint8_t hour, uint8_t minute);
    void setBrightness(uint8_t brightness);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t color);
    void startLoading();
    void stopLoading();
  private:
    void displayNumberAtPosition(uint8_t number, uint8_t position);
    void displayWordAt(uint8_t index);
    float deg2rad(int deg);
    double brightness(int deg);
    void loopLoading();
    bool _isLoading;
    unsigned long _loadingStarted;
    unsigned long _lastLoadingFrame;
    static uint8_t displayMapping[9][9];
    static uint8_t numberMapping[10][9][4];
    static uint8_t numberMappingRows();
    static uint8_t numberMappingCols();
    static uint8_t wordMapping[23][6];
};

#endif
