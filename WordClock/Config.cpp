#include <EEPROM.h>
#include "Config.h"

ConfigStorageStruct ClockConfig = {
  CONFIG_VERSION,
  "",
  "",
  0,
  20,
  170
};

Config::Config() {
  EEPROM.begin(512);
  this->read();
}

void Config::read() {
  if (EEPROM.read(CONFIG_START) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]) {
    for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
      *((char*)&ClockConfig + t) = EEPROM.read(CONFIG_START + t);
    }
  }
}

void Config::save() {
  for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
    EEPROM.write(CONFIG_START + t, *((char*)&ClockConfig + t));
  }
}

