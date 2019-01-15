#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "API.h"
#include "Wifi.h"
#include "Config.h"
extern struct ConfigStorageStruct ClockConfig;

#define SERVER_DOMAIN "wordclock"

class WebServer {
    static Wifi* _wifi;
    static API* _api;
  public:
    WebServer();
    void setup(Wifi *wifi, API *api);
    void loop();
  private:
    static void handleConfigSet();
    static void handleConfigGet();
    static void handleNotFound();
};
#endif
