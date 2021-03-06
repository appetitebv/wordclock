#include "API.h"

HTTPClient http;

Time LastSync = {
  0,0,0,0,0,0
};

API::API() {
}

Clock* API::_clock;
SunsetSunrise* API::_sunsetSunrise;

void API::setup(Clock *clock, SunsetSunrise *sunsetSunrise) {
  Serial.println("API::setup");
  _clock = clock;
  _sunsetSunrise = sunsetSunrise;
}

void API::loop() {
  // Sync at 03:05 AM
  Time time = _clock->getTime();
  if ((LastSync.day != time.day && time.hour == 3 && time.minute == 5) || (LastSync.year == 0)) {
    this->sync();
  }
}

void API::sync() {
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
      _clock->setTime(currentTime);

      // Parse sunrise
      auto sunriseString = payload["sunrise"].as<char*>();
      Time sunrise = this->parseTime(sunriseString);

      // Parse sunset
      auto sunsetString = payload["sunset"].as<char*>();
      Time sunset = this->parseTime(sunsetString);

      _sunsetSunrise->set(sunrise, sunset);
      
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
}

Time API::lastSync() {
  return LastSync;
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
  uint16_t year = ((string[0]-'0')*1000)+((string[1]-'0')*100)+((string[2]-'0')*10)+(string[3]-'0');
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
