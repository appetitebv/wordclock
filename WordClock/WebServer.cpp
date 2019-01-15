#include "WebServer.h"

ESP8266WebServer server(80);

WebServer::WebServer() {
}

Wifi* WebServer::_wifi;
API* WebServer::_api;

void WebServer::setup(Wifi *wifi, API *api) {
  Serial.println("Server::setup");
  _wifi = wifi;
  _api = api;
  if (!MDNS.begin(SERVER_DOMAIN)) {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/config/set", HTTP_POST, handleConfigSet);
  server.on("/config/get", HTTP_GET, handleConfigGet);
  server.onNotFound(this->handleNotFound);
  server.begin();
}

void WebServer::loop() {
  server.handleClient();
  MDNS.update();
}

void WebServer::handleConfigSet() {
  const size_t capacity = 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 180;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& payload = jsonBuffer.parseObject(server.arg("plain"));

  if (!payload.success()) {
    server.send(500);
    return;
  }

  bool reconnect=false;

   if (payload.containsKey("wifi")) {
    const char *ssid = payload["wifi"]["ssid"];
    const char *pwd = payload["wifi"]["pwd"];
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PWD:");
    Serial.println(pwd);
    strcpy(ClockConfig.ssid, ssid);
    strcpy(ClockConfig.pwd, pwd);
    reconnect=true;
   }
   if (payload.containsKey("clock")) {
    ClockConfig.clockColor = payload["clock"]["color"];
    ClockConfig.clockBrightnessDay = payload["clock"]["brightness"]["day"];
    ClockConfig.clockBrightnessNight = payload["clock"]["brightness"]["night"];
   }
   if (payload.containsKey("gps")) {
    ClockConfig.lat = payload["gps"]["lat"];
    ClockConfig.lng = payload["gps"]["lng"];
   }
   Config::save();
   server.send(200);

   if (reconnect) {
    _wifi->connectToWifi();
    _api->sync();
   }
}

void WebServer::handleConfigGet() {
  Serial.println("Sending JSON");
  const size_t capacity = 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 180;
  DynamicJsonBuffer jsonBuffer(capacity);
  
  JsonObject& payload = jsonBuffer.createObject();
  
  JsonObject& wifi = payload.createNestedObject("wifi");
  wifi["ssid"] = ClockConfig.ssid;
  wifi["pwd"] = ClockConfig.pwd;
  
  JsonObject& clock = payload.createNestedObject("clock");
  clock["color"] = ClockConfig.clockColor;
  
  JsonObject& brightness = clock.createNestedObject("brightness");
  brightness["day"] = ClockConfig.clockBrightnessDay;
  brightness["night"] = ClockConfig.clockBrightnessNight;
  
  String json;
  payload.prettyPrintTo(json);

  Serial.println(json);
  
  server.send(200, "application/json", json);
}

void WebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
