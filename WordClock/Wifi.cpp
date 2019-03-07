#include "Wifi.h"

Wifi::Wifi() {
}

void Wifi::setup() {
  _isConnecting = false;
  _isAPModeActive = false;
  Serial.println("Wifi::setup");
  if (strcmp(ClockConfig.ssid, "") == 0) {
    this->startAccessPoint();
  } else {
    this->connectToWifi();
  }
}

void Wifi::loop() {
  this->connectToWifi();
}

void Wifi::startAccessPoint() {
  Serial.println("Start AP");
  char ssid[24];
  this->accessPointSSID(ssid);
  WiFi.softAP(ssid, NULL, 11);
  _isAPModeActive = true;
}

// Connect to WiFi
void Wifi::connectToWifi() {
  if (_isAPModeActive) {
    return;
  }
  if (!_isConnecting && !this->wifiConnected()) {
    Serial.println("Should connect");
    if (strcmp(ClockConfig.ssid, "") == 0 || strcmp(ClockConfig.pwd, "") == 0) {
      Serial.println("ssid or pwd empty");
      this->startAccessPoint();
      return;
    }
    _isConnecting = true;
    _connectingStarted = millis();
    Serial.println(_connectingStarted);
    WiFi.disconnect();
    Serial.print("Connect to: ");
    Serial.println(ClockConfig.ssid);
    WiFi.begin(ClockConfig.ssid, ClockConfig.pwd);
    uint8_t wifiConnectWait = 0;
  } else if (WiFi.status() != WL_CONNECTED) {
    if ((millis()-_connectingStarted) >= 30000) {
      Serial.println("Connecting timeout!");
      this->startAccessPoint();
      _isConnecting = false;
    }
  } else if (_isConnecting) {
    Serial.println("Connected! stop connecting");
    // Disable AP.
    WiFi.softAPdisconnect(true);
    
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    _isConnecting = false;
  }
}

void Wifi::reconnect() {
  _isConnecting = false;
  _isAPModeActive = false;
  Serial.println("Reconnect");
  WiFi.disconnect();
  this->connectToWifi();
}

// Verify that WiFi is connected
bool Wifi::wifiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  return true;
}

void Wifi::accessPointSSID(char *ssid) {
  strcpy(ssid, WIFI_SSID);
  strcat(ssid, "-");
  auto chipid = ESP.getChipId();
  strcat(ssid, String(chipid).c_str());
}

