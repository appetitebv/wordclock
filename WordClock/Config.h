#ifndef Config_h
#define Config_h

#include <stdint.h>
#include <Arduino.h>

#define CONFIG_VERSION "001"
#define CONFIG_START 0

struct ConfigStorageStruct {
  char version[4];
  char ssid[24];
  char pwd[16];
  uint32_t clockColor;
  uint8_t clockBrightnessNight;
  uint8_t clockBrightnessDay;
};

class Config {
  public:
    Config();
    void setup();
  private:
    void read();
    void save();
};

#endif
