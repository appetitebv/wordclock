#include <EEPROM.h>
#include "Config.h"

ConfigStorageStruct ClockConfig = {
  CONFIG_VERSION,
  CONFIG_FIRM_VERSION,
  CONFIG_SSID,
  CONFIG_PWD,
  CONFIG_COLOR,
  CONFIG_BRIGHTNESS_NIGHT,
  CONFIG_BRIGHTNESS_DAY,
  CONFIG_LAT,
  CONFIG_LNG
};

Config::Config() {
}

void Config::setup() {
  Serial.println("Config::Config");
  EEPROM.begin(512);
  this->read();

  // After auto-update, update firmwareVersion
  if (ClockConfig.firmwareVersion != CONFIG_FIRM_VERSION) {
    ClockConfig.firmwareVersion = CONFIG_FIRM_VERSION;
    this->save();
  }
}

void Config::read() {
  Serial.println("config::read");
  // Verify correct version is in EEPROM
  if (EEPROM.read(CONFIG_START) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]) {
        Serial.println("Read config from EEPROM");
        // Read the saved ClockConfig from EEPROM
    for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
      *((char*)&ClockConfig + t) = EEPROM.read(CONFIG_START + t);
    }
  } else {
    Serial.println("Config not in EEPROM.");
    // we should save the config as soon as we're able to update it.
    // this->save();
  }
}

void Config::save() {
  for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
    EEPROM.write(CONFIG_START + t, *((char*)&ClockConfig + t));
  }
}

