#include "API.h"

HTTPClient http;

API::API() {
}

void API::setup() {
  Serial.println("API::setup");
  this->connectToWifi();
}

void API::loop() {
  if (this->wifiConnected() == false) {
    this->connectToWifi();
  }
}

void API::sync(Clock *clock) {
  // Allocate json buffer
  const int encodeCapacity = JSON_OBJECT_SIZE(3);
  StaticJsonBuffer<encodeCapacity> encodeBuffer;

  // Create json object
  JsonObject& obj = encodeBuffer.createObject();
  obj["lat"] = ClockConfig.lat;
  obj["long"] = ClockConfig.lng;
  obj["currentVersion"] = ClockConfig.firmwareVersion;
  
  // Buffer for output 
  char output[128];

  // Write json object to buffer
  obj.printTo(output, 128);
  
  http.begin("http://wordclock.nl/api/status");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(output);
  Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();

    Serial.println(response);

    // Allocate decoding buffer
    const size_t decodeSize = JSON_OBJECT_SIZE(4) + 190;
    DynamicJsonBuffer decodeBuffer(decodeSize);

    // Parse the response payload
    JsonObject& payload = decodeBuffer.parseObject(response);

    if (payload.success()) {
      auto currentTime = payload["currentTime"].as<char*>();
      
      // Extract the time
      uint8_t hour = ((currentTime[11]-'0')*10)+(currentTime[12]-'0');
      uint8_t minute = ((currentTime[14]-'0')*10)+(currentTime[15]-'0');
      uint8_t second = ((currentTime[17]-'0')*10)+(currentTime[18]-'0');
      clock->setTime(hour, minute, second);
    } else {
      Serial.println("Decoding json failed");
    }
  }
  http.end();
}

// Connect to WiFi
void API::connectToWifi() {
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
bool API::wifiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    return false;
  }
  return true;
}

