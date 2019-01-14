#include "WebServer.h"

ESP8266WebServer server(80);

WebServer::WebServer() {
}

void WebServer::setup() {
  Serial.println("Server::setup");
  if (!MDNS.begin(SERVER_DOMAIN)) {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/", this->handleRoot);
  server.onNotFound(this->handleNotFound);
  server.begin();
}

void WebServer::loop() {
  server.handleClient();
  MDNS.update();
}

void WebServer::handleRoot() {
  Serial.println("handleRoot");
  server.send(200, "application/json", "{\"status\": \"ok\"}");
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
