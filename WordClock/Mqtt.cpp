#include "Mqtt.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "wordclock/status";
const char* MQTT_LIGHT_COMMAND_TOPIC = "wordclock/switch";
const char* MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC = "wordclock/brightness/status";
const char* MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC = "wordclock/brightness/set";
const char* MQTT_LIGHT_RGB_STATE_TOPIC = "wordclock/rgb/status";
const char* MQTT_LIGHT_RGB_COMMAND_TOPIC = "wordclock/rgb/set";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

Mqtt::Mqtt() {
}

// Function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // Concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  
//  // Handle message topic
//  if (String(MQTT_LIGHT_COMMAND_TOPIC).equals(p_topic)) {
//    // test if the payload is equal to "ON" or "OFF"
//    if (payload.equals(String(LIGHT_ON))) {
//      if (m_light_state != true) {
//        m_light_state = true;
//        setLightState();
//        publishLightState();
//      }
//    } else if (payload.equals(String(LIGHT_OFF))) {
//      if (m_light_state != false) {
//        m_light_state = false;
//        setLightState();
//        publishLightState();
//      }
//    }
//  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("wordclock", ClockConfig.mqttUsername, ClockConfig.mqttPasswd)) {
      Serial.println("INFO: connected");
      // Once connected, publish an announcement...
      //publishLightState();
      // ... and resubscribe
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
    }
  }
}


void Mqtt::setup() {
  client.setServer(ClockConfig.mqttHost, ClockConfig.mqttPort);
  client.setCallback(callback);
}

void Mqtt::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
