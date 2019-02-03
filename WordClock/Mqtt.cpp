#include "Mqtt.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "wordclock/status";
const char* MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC = "wordclock/brightness/status";
const char* MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC = "wordclock/brightness/set";
const char* MQTT_LIGHT_RGB_STATE_TOPIC = "wordclock/rgb/status";
const char* MQTT_LIGHT_RGB_COMMAND_TOPIC = "wordclock/rgb/set";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

Display* _display;

Mqtt::Mqtt() {
}

void setRgbState(String payload) {
  String r2 = payload.substring(0,3);
  String g2 = payload.substring(3,6);
  String b2 = payload.substring(6);

  int r = payload.substring(0,3).toInt();
  int g = payload.substring(3,6).toInt();
  int b = payload.substring(6).toInt();

  _display->setColor(r, g, b);
}

void setBrightness(String payload) {
  int brightness = payload.toInt();
  _display->setBrightness(brightness);
}

// function called to publish the state of the led (on/off)
void publishLightState() {
    client.publish(MQTT_LIGHT_STATE_TOPIC, "on", true);
}

// Function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // Concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }

  Serial.println(p_topic);
  Serial.println(payload);
  
//  // Handle message topic
  if (String(MQTT_LIGHT_RGB_COMMAND_TOPIC).equals(p_topic)) { 
    setRgbState(payload);
  } else if (String(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC).equals(p_topic)) { 
    setBrightness(payload);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("wordclock", ClockConfig.mqttUsername, ClockConfig.mqttPasswd)) {
      Serial.println("INFO: connected");
      // Once connected, publish an announcement...
      publishLightState();
      // ... and resubscribe
      client.subscribe(MQTT_LIGHT_RGB_COMMAND_TOPIC);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
    }
  }
}


void Mqtt::setup(Display *display) {
  _display = display;
  
  client.setServer(ClockConfig.mqttHost, ClockConfig.mqttPort);
  client.setCallback(callback);
}

void Mqtt::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
