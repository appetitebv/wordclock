#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

#define SERVER_DOMAIN "wordclock"

class WebServer {
  public:
    WebServer();
    void setup();
    void loop();
  private:
    static void handleConfigSet();
    static void handleConfigGet();
    static void handleNotFound();
};
#endif
