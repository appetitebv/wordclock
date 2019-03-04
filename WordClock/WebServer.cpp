#include "WebServer.h"

ESP8266WebServer server(80);

WebServer::WebServer() {
}

Wifi* WebServer::_wifi;
API* WebServer::_api;
SunsetSunrise* WebServer::_sunsetSunrise;
Display* WebServer::_display;
Mqtt* WebServer::_mqtt;
Clock* WebServer::_clock;

const char* PROGMEM rootHTML = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1,maximum-scale=1\"><title>Word Clock</title><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css\"></head><body><div class=\"container\"><h1>Word Clock</h1><div class=\"form-group\"><label for=\"ssid\">Network</label><input type=\"text\" class=\"form-control\" id=\"ssid\" placeholder=\"SSID\"></div><div class=\"form-group\"><label for=\"password\">Password</label><input type=\"password\" class=\"form-control\" id=\"password\" placeholder=\"Password\"></div><button type=\"submit\" class=\"btn btn-primary\" id=\"save\">Save</button></div><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script><script language=\"javascript\" type=\"text/javascript\">$(\"#save\").click(function(){$.ajax({url:\"/config/set\",type:\"POST\",contentType:\"application/json\",data:JSON.stringify({wifi:{ssid:$(\"#ssid\").val(),pwd:$(\"#password\").val()}}),dataType:\"json\"});});</script></body></html>";

void WebServer::setup(Wifi *wifi, API *api, SunsetSunrise *sunsetSunrise, Display *display, Mqtt *mqtt, Clock *clock) {
  Serial.println("Server::setup");
  _wifi = wifi;
  _api = api;
  _sunsetSunrise = sunsetSunrise;
  _display = display;
  _mqtt = mqtt;
  _clock = clock;
  if (!MDNS.begin(SERVER_DOMAIN)) {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/", HTTP_GET, handleRoot);
  server.on("", HTTP_GET, handleRoot);
  server.on("/config/set", HTTP_POST, handleConfigSet);
  server.on("/color/set", HTTP_POST, handleColorSet);
  server.on("/brightness/set", HTTP_POST, handleBrightnessSet);
  server.on("/config/get", HTTP_GET, handleConfigGet);
  server.onNotFound(this->handleNotFound);
  server.begin();
}

void WebServer::loop() {
  server.handleClient();
  MDNS.update();
}

void WebServer::handleRoot() {
  server.sendHeader("Content-Type", "text/html");
  server.sendContent_P(rootHTML);
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
    strcpy(ClockConfig.ssid, ssid);
    strcpy(ClockConfig.pwd, pwd);
    reconnect=true;
  }
  if (payload.containsKey("clock")) {
    ClockConfig.clockColor = payload["clock"]["color"];
    _display->setColor(ClockConfig.clockColor);
    _mqtt->publishColor(ClockConfig.clockColor);
    
    ClockConfig.clockBrightnessDay = payload["clock"]["brightness"]["day"];
    ClockConfig.clockBrightnessNight = payload["clock"]["brightness"]["night"];
    // TODO: How to know here wether to set day or night brightness?
    _display->setBrightness(ClockConfig.clockBrightnessDay);
    _mqtt->publishBrightness(ClockConfig.clockBrightnessDay);
  }
  if (payload.containsKey("gps")) {
    ClockConfig.lat = payload["gps"]["lat"];
    ClockConfig.lng = payload["gps"]["lng"];
  }
  if (payload.containsKey("mqtt")) {
    bool mqttEnabled = payload["mqtt"]["enabled"];
    ClockConfig.mqttEnabled = mqttEnabled;
    if (mqttEnabled) {
      const char *mqttHost = payload["mqtt"]["host"];
      const char *mqttUsername = payload["mqtt"]["username"];
      const char *mqttPasswd = payload["mqtt"]["passwd"];

      strcpy(ClockConfig.mqttHost, mqttHost);
      strcpy(ClockConfig.mqttUsername, mqttUsername);
      strcpy(ClockConfig.mqttPasswd, mqttPasswd);
      ClockConfig.mqttPort = payload["mqtt"]["port"];
    } else {
      strcpy(ClockConfig.mqttHost, "");
      strcpy(ClockConfig.mqttUsername, ""); 
      strcpy(ClockConfig.mqttPasswd, "");
    }
  }
   
   Config::save();
   server.send(200);

   if (reconnect) {
    _wifi->reconnect();
   }
}

void WebServer::handleColorSet() {
  const size_t capacity = JSON_OBJECT_SIZE(1) + 40;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& payload = jsonBuffer.parseObject(server.arg("plain"));

  if (!payload.success()) {
    server.send(500);
    return;
  }
  
  if (payload.containsKey("color")) {
    uint32_t color = payload["color"];
    _display->setColor(color);
    _mqtt->publishColor(color);
  }
  
  server.send(200);
}

void WebServer::handleBrightnessSet() {
  const size_t capacity = JSON_OBJECT_SIZE(1) + 40;
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& payload = jsonBuffer.parseObject(server.arg("plain"));

  if (!payload.success()) {
    server.send(500);
    return;
  }

  if (payload.containsKey("brightness")) {
    uint8_t brightness = payload["brightness"];
    _display->setBrightness(brightness);
    _mqtt->publishBrightness(brightness);
  }
  
  server.send(200);
}

void WebServer::handleConfigGet() {
  Serial.println("Sending JSON");
  const size_t capacity = 2*JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(3) + 162;
  DynamicJsonBuffer jsonBuffer(capacity);
  
  JsonObject& payload = jsonBuffer.createObject();
  payload["version"] = ClockConfig.firmwareVersion;

  String bootTime;
  WebServer::printTime(_clock->getBootTime(), bootTime);
  payload["bootTime"] = bootTime;
  
  JsonObject& wifi = payload.createNestedObject("wifi");
  wifi["ssid"] = ClockConfig.ssid;
  
  JsonObject& clock = payload.createNestedObject("clock");
  clock["color"] = ClockConfig.clockColor;
  clock["currentColor"] = _display->getCurrentColor();
  
  JsonObject& brightness = clock.createNestedObject("brightness");
  brightness["day"] = ClockConfig.clockBrightnessDay;
  brightness["night"] = ClockConfig.clockBrightnessNight;
  brightness["current"] = _display->getCurrentBrightness();

  JsonObject& gps = payload.createNestedObject("gps");
  gps["lat"] = ClockConfig.lat;
  gps["lng"] = ClockConfig.lng;

  JsonObject& mqtt = payload.createNestedObject("mqtt");
  mqtt["enabled"] = ClockConfig.mqttEnabled;
  mqtt["host"] = ClockConfig.mqttHost;
  mqtt["username"] = ClockConfig.mqttUsername;

  JsonObject& sunriseSunset = payload.createNestedObject("sunriseSunset");
  String sunrise;
  WebServer::printTime(_sunsetSunrise->sunrise(), sunrise);
  sunriseSunset["sunrise"] = sunrise;
  
  String sunset;
  WebServer::printTime(_sunsetSunrise->sunset(), sunset);
  sunriseSunset["sunset"] = sunset;

  String lastSync;
  WebServer::printTime(_api->lastSync(), lastSync);
  sunriseSunset["lastSync"] = lastSync;

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

void WebServer::printTime(Time time, String &output) {
  output=String("");
  output+=time.year;
  output+="-";
  if (time.month<10) {
    output+="0";
  }
  output+=time.month;
  output+="-";
  if (time.day < 10) {
    output+="0";
  }
  output+=time.day;
  output+=" ";
  if (time.hour < 10) {
    output+="0";
  }
  output+=time.hour;
  output+=":";
  if (time.minute < 10) {
    output+="0";
  }
  output+=time.minute;
  output+=":";
  if (time.second < 10) {
    output+="0";
  }
  output+=time.second;
}
