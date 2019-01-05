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

void API::sync(Clock *clock, SunsetSunrise *sunsetSunrise) {
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
  http.setTimeout(3000); // just in case
  int httpCode = http.POST(output);
  Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();

    Serial.println(response);

    // Allocate decoding buffer
    const size_t decodeSize = JSON_OBJECT_SIZE(4) + 220;
    DynamicJsonBuffer decodeBuffer(decodeSize);

    // Parse the response payload
    JsonObject& payload = decodeBuffer.parseObject(response);

    if (payload.success()) {
      // Parse current time
      auto currentTimeString = payload["currentTime"].as<char*>();
      Time currentTime = this->parseTime(currentTimeString);
      clock->setTime(currentTime);

      // Parse sunrise
      auto sunriseString = payload["sunrise"].as<char*>();
      Time sunrise = this->parseTime(sunriseString);

      // Parse sunset
      auto sunsetString = payload["sunset"].as<char*>();
      Time sunset = this->parseTime(sunsetString);

      sunsetSunrise->set(sunrise, sunset);
      
    } else {
      Serial.println("Decoding json failed");
    }
  }
  http.end();

  // Retry
  if (httpCode != HTTP_CODE_OK) {
    // Double check if WiFi is working
    if (!this->wifiConnected()) {
      this->connectToWifi();
    }
    this->sync(clock, sunsetSunrise);
  }
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

Time API::parseTime(const char *string) {
  uint8_t hour = ((string[11]-'0')*10)+(string[12]-'0');
  uint8_t minute = ((string[14]-'0')*10)+(string[15]-'0');
  uint8_t second = ((string[17]-'0')*10)+(string[18]-'0');
  Time time {
    hour,
    minute,
    second
  };
  return time;
}

