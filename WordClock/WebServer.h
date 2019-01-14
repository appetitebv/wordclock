#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SERVER_DOMAIN "wordclock"

class WebServer {
  public:
    WebServer();
    void setup();
    void loop();
  private:
    static void handleRoot();
    static void handleNotFound();
};
#endif
