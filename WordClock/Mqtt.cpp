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

const char* LIGHT_ON = "on";
const char* LIGHT_OFF = "off";

boolean m_light_state = false;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

Mqtt::Mqtt() {
}

int convertToInt(char upper,char lower) {
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal >64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal >64 ? lVal - 55 : lVal - 48;
  return uVal + lVal;
}

uint32_t convertTo32Bit(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

void setRgbState(String payload) {
  char c[7]; 
  payload.toCharArray(c,8);

  int r = convertToInt(c[1],c[2]);
  int g = convertToInt(c[3],c[4]);
  int b = convertToInt(c[5],c[6]);

  ClockConfig.clockColor = convertTo32Bit(r, g, b);
}

void publishRgbState() {
  
}

void setLightState() {
 // TODO
}

// function called to publish the state of the led (on/off)
void publishLightState() {
  if (m_light_state) {
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_ON, true);
  } else {
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_OFF, true);
  }
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
  if (String(MQTT_LIGHT_COMMAND_TOPIC).equals(p_topic)) { 
    if (payload.equals(String(LIGHT_ON))) {
      if (m_light_state != true) {
        m_light_state = true;
        setLightState();
        publishLightState();
      }
    } else if (payload.equals(String(LIGHT_OFF))) {
      if (m_light_state != false) {
        m_light_state = false;
        setLightState();
        publishLightState();
      }
    }
  } else if (String(MQTT_LIGHT_RGB_COMMAND_TOPIC).equals(p_topic)) { 
    setRgbState(payload);
    publishRgbState();
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
      //publishLightState();
      // ... and resubscribe
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC);
      client.subscribe(MQTT_LIGHT_RGB_COMMAND_TOPIC);
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
