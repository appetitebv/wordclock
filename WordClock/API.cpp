#include "API.h"

HTTPClient http;
Wifi wifi;

Time LastSync = {
  0,0,0,0,0,0
};

API::API() {
}

void API::setup() {
  Serial.println("API::setup");
  wifi.setup();
}

void API::loop(Clock *clock, SunsetSunrise *sunsetSunrise) {
  // Sync at 01:00 AM
  Time time = clock->getTime();
  if (LastSync.day != time.day && time.hour == 1 && time.minute == 0) {
    this->sync(clock, sunsetSunrise);
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
    const size_t decodeSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 220;
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
      
      // Save last sync
      LastSync = currentTime;

      if (payload.containsKey("newFirmware")) {
        const char* host = payload["newFirmware"]["host"];
        const char* path = payload["newFirmware"]["path"];
        this->updateFirmware(host, path);
      } else {
        Serial.println("No new firmware available");
      }
      
    } else {
      Serial.println("Decoding json failed");
    }
  }
  http.end();

  // Retry
  if (httpCode != HTTP_CODE_OK) {
    // Double check if WiFi is working
    if (!wifi.connected()) {
      wifi.connect();
    }
    this->sync(clock, sunsetSunrise);
  }
}

void API::updateFirmware(const char* host, const char* path) {
    Serial.print("Updating firmware from host: ");
    Serial.print(host);
    Serial.print(" with path: ");
    Serial.println(path);
    ESPhttpUpdate.update(host, 80, path);
    Serial.println("Done updating.");
}

Time API::parseTime(const char *string) {
  uint8_t year = ((string[0]-'0')*1000)+((string[1]-'0')*100)+((string[2]-'0')*10)+(string[3]-'0');
  uint8_t month = ((string[5]-'0')*10)+(string[6]-'0');
  uint8_t day = ((string[8]-'0')*10)+(string[9]-'0');
  uint8_t hour = ((string[11]-'0')*10)+(string[12]-'0');
  uint8_t minute = ((string[14]-'0')*10)+(string[15]-'0');
  uint8_t second = ((string[17]-'0')*10)+(string[18]-'0');
  Time time {
    year,
    month,
    day,
    hour,
    minute,
    second
  };
  return time;
}
