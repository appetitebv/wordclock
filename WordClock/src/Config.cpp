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
  CONFIG_LNG,
  CONFIG_MQTT_ENABLED,
  CONFIG_MQTT_HOST,
  CONFIG_MQTT_PORT,
  CONFIG_MQTT_USERNAME,
  CONFIG_MQTT_PASSWD
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
  Serial.println("Config::read");
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
    Config::upgradeFromPreviousVersion();
  }
}

void Config::save() {
  Serial.println("Save config");
  for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
    EEPROM.write(CONFIG_START + t, *((char*)&ClockConfig + t));
  }
  EEPROM.commit();
}

void Config::upgradeFromPreviousVersion() {
  Serial.println("Trying to upgrade");
  if (EEPROM.read(CONFIG_START) == PREVIOUS_CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == PREVIOUS_CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == PREVIOUS_CONFIG_VERSION[2]) {
    Serial.println("Found old config");
    // Read the saved ClockConfig from EEPROM
    for (unsigned int t=0; t<sizeof(ClockConfig); t++) {
      *((char*)&ClockConfig + t) = EEPROM.read(CONFIG_START + t);
    }

    // Update config version
    strcpy(ClockConfig.configVersion, CONFIG_VERSION);

    // Set new variables
    ClockConfig.mqttEnabled = CONFIG_MQTT_ENABLED;
    strcpy(ClockConfig.mqttHost, CONFIG_MQTT_HOST);
    ClockConfig.mqttPort = CONFIG_MQTT_PORT;
    strcpy(ClockConfig.mqttUsername, CONFIG_MQTT_USERNAME);
    strcpy(ClockConfig.mqttPasswd, CONFIG_MQTT_PASSWD);

    Config::save();
    Serial.println("Converted to new config version");
  }
}

