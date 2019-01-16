#include "Wifi.h"

Wifi::Wifi() {
}

void Wifi::setup() {
  Serial.println("Wifi::setup");
  if (strcmp(ClockConfig.ssid, "") == 0) {
    this->startAP();
  } else {
    this->connectToWifi();
  }
}

void Wifi::loop() {
  if (this->wifiConnected() == false) {
    this->connectToWifi();
  }
}

void Wifi::startAP() {
  Serial.println("Start AP");
  WiFi.softAP(WIFI_SSID, NULL, 11);
}

// Connect to WiFi
void Wifi::connectToWifi() {
  if (strcmp(ClockConfig.ssid, "") == 0) {
    return;
  }
  WiFi.disconnect();
  Serial.print("Connect to: ");
  Serial.println(ClockConfig.ssid);
  WiFi.begin(ClockConfig.ssid, ClockConfig.pwd);
  uint8_t wifiConnectWait = 0;
  while (WiFi.status() != WL_CONNECTED && wifiConnectWait < 20) {
    // TODO: Print some status to the display?
    Serial.print(".");
    delay(500);
    wifiConnectWait = wifiConnectWait+1;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    // Disable AP.
    WiFi.softAPdisconnect(true);
    
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 
  } else {
    Serial.println("Unable to connect to WiFi.");
    this->startAP();
  }
}

// Verify that WiFi is connected
bool Wifi::wifiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  return true;
}
