#include "Mqtt.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "wordclock/status";
const char* MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC = "wordclock/brightness/status";
const char* MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC = "wordclock/brightness/set";
const char* MQTT_LIGHT_RGB_STATE_TOPIC = "wordclock/rgb/status";
const char* MQTT_LIGHT_RGB_COMMAND_TOPIC = "wordclock/rgb/set";

Mqtt::Mqtt() {
}

Display* Mqtt::_display;

void Mqtt::setup(Display *display) {
  _display = display;
  
  client.setServer(ClockConfig.mqttHost, ClockConfig.mqttPort);
  client.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
}

void Mqtt::loop() {
  if (!client.connected()) {
    this->reconnect();
  }
  client.loop();
}

void Mqtt::publishBrightness(uint8_t brightness) {
  if (ClockConfig.mqttEnabled == true && client.connected() == true) {
    char payload[4];
    sprintf(payload, "%d", brightness);

    Serial.print("Brightness payload: ");
    Serial.println(payload);
    
    client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC, payload, true);
    Serial.println("published");
  }
}

void Mqtt::publishColor(uint32_t color) {
  if (ClockConfig.mqttEnabled == true && client.connected() == true) {
    uint8_t colorR = ((color >> 16) & 0xFF);
    uint8_t colorG = ((color >> 8) & 0xFF);
    uint8_t colorB = ((color >> 0) & 0xFF);
    
    char r[4],g[4],b[4];
    sprintf(r,"%03d",colorR);
    sprintf(g,"%03d",colorG);
    sprintf(b,"%03d",colorB);
    
    char payload[10];
    strcpy(payload, r);
    strcat(payload, g);
    strcat(payload, b);
    
    Serial.print("Color Payload: ");
    Serial.println(payload);
    
    client.publish(MQTT_LIGHT_RGB_STATE_TOPIC, payload, true);
    Serial.println("Published");
  }
}

void Mqtt::setRgbState(String payload) {
  uint8_t r = payload.substring(0,3).toInt();
  uint8_t g = payload.substring(3,6).toInt();
  uint8_t b = payload.substring(6).toInt();
  _display->setColor(r, g, b);
}

void Mqtt::setBrightness(String payload) {
  uint8_t brightness = payload.toInt();
  _display->setBrightness(brightness);
}

// function called to publish the state of the led (on/off)
void Mqtt::publishLightState() {
    client.publish(MQTT_LIGHT_STATE_TOPIC, "on", true);
}

// Function called when a MQTT message arrived
void Mqtt::callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // Concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }

  Serial.println(p_topic);
  Serial.println(payload);
  
  // Handle message topic
  if (String(MQTT_LIGHT_RGB_COMMAND_TOPIC).equals(p_topic)) { 
    this->setRgbState(payload);
  } else if (String(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC).equals(p_topic)) { 
    this->setBrightness(payload);
  }

}

void Mqtt::reconnect() {
  Serial.println("INFO: Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("wordclock", ClockConfig.mqttUsername, ClockConfig.mqttPasswd)) {
    Serial.println("INFO: connected");
    // Once connected, publish an announcement...
    this->publishLightState();
    // ... and resubscribe
    client.subscribe(MQTT_LIGHT_RGB_COMMAND_TOPIC);
    client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC);
  } else {
    Serial.print("ERROR: failed, rc=");
    Serial.print(client.state());
  }
}
