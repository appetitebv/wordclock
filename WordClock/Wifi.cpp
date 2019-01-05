#include "Wifi.h"

Wifi::Wifi() {
}

void Wifi::setup() {
  Serial.println("Wifi::setup");
  this->connect();
}

void Wifi::loop() {
  if (this->connected() == false) {
    this->connect();
  }
}

// Connect to WiFi
void Wifi::connect() {
  WiFi.begin(ClockConfig.ssid, ClockConfig.pwd);
  while (WiFi.status() != WL_CONNECTED) {
    // TODO: Print some status to the display?
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
}

// Verify that WiFi is connected
bool Wifi::connected() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    return false;
  }
  return true;
}
