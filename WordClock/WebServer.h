#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "API.h"
#include "Wifi.h"
#include "Config.h"
#include "SunsetSunrise.h"
#include "Mqtt.h"
extern struct ConfigStorageStruct ClockConfig;

#define SERVER_DOMAIN "wordclock"

class WebServer {
    static Wifi* _wifi;
    static API* _api;
    static SunsetSunrise* _sunsetSunrise;
    static Display* _display;
    static Mqtt* _mqtt;
  public:
    WebServer();
    void setup(Wifi *wifi, API *api, SunsetSunrise *sunsetSunrise, Display *display, Mqtt *mqtt);
    void loop();
  private:
    static void handleRoot();
    static void handleConfigSet();
    static void handleConfigGet();
    static void handleNotFound();
    static void printTime(Time time, String &output);
};
#endif
