#include "WebServer.h"

ESP8266WebServer server(80);

WebServer::WebServer() {
}

void WebServer::setup() {
  Serial.println("Server::setup");
  if (!MDNS.begin(SERVER_DOMAIN)) {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/config/set", HTTP_POST, this->handleConfigSet);
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

   if (payload.containsKey("ssid")) {
    strcpy(ClockConfig.ssid, payload["ssid"]);
   }
   if (payload.containsKey("pwd")) {
    strcpy(ClockConfig.pwd, payload["pwd"]);
   }
   if (payload.containsKey("clock")) {
    ClockConfig.clockColor = payload["clock"]["color"];
    ClockConfig.clockBrightnessNight = payload["clock"]["brightness"]["day"];
    ClockConfig.clockBrightnessDay = payload["clock"]["brightness"]["night"];
   }
   if (payload.containsKey("gps")) {
    ClockConfig.lat = payload["gps"]["lat"];
    ClockConfig.lng = payload["gps"]["lng"];
   }
   Config::save();
   server.send(200);
}

void WebServer::handleConfigGet() {
  // TODO
  WebServer::handleNotFound();
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
