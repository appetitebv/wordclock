#ifndef Config_h
#define Config_h

#include <stdint.h>
#include <Arduino.h>

#define CONFIG_VERSION "001"
#define CONFIG_SSID ""
#define CONFIG_PWD ""
#define CONFIG_FIRM_VERSION 5
#define CONFIG_COLOR 8355711        // R127 G127 B127
#define CONFIG_BRIGHTNESS_NIGHT 20  // 0-256
#define CONFIG_BRIGHTNESS_DAY 170   // 0-256
#define CONFIG_LAT 52.009620
#define CONFIG_LNG 4.693660
#define CONFIG_MQTT_ENABLED false
#define CONFIG_MQTT_HOST ""
#define CONFIG_MQTT_USERNAME ""
#define CONFIG_MQTT_PASSWD ""
 
#define CONFIG_START 32

struct ConfigStorageStruct {
  char configVersion[4];
  uint8_t firmwareVersion;
  char ssid[24];
  char pwd[16];
  uint32_t clockColor;
  uint8_t clockBrightnessNight;
  uint8_t clockBrightnessDay;
  double lat;
  double lng;
  bool mqttEnabled;
  char mqttHost[24];
  char mqttUsername[20];
  char mqttPasswd[20];
};

class Config {
  public:
    Config();
    void setup();
    static void save();
  private:
    static void read();
};

#endif
