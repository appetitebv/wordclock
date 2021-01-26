# WordClock
## RTC DS3231
Connected via: I2C-bus on pins D1 (GPIO5/SCL) and D2 (GPIO4/SDA)
## WS2812 LED's
Connected via: D4 (GPIO2)
## Libraries used
- https://github.com/bblanchon/ArduinoJson
- https://github.com/NorthernWidget/DS3231
- https://github.com/adafruit/Adafruit_NeoPixel
## API
HTTP POST to path: ```/config/set```
With JSON:
```json
{
"wifi": {
"ssid": "123456789012345678901234",
"pwd": "1234567890123456"
},
"clock": {
"color": 4294967295,
"brightness": {
"day": 255,
"night": 255
}
},
"gps": {
"lat": 18446744073709551615,
"lng": 18446744073709551615
},
"mqtt": {
"enabled": true,
"host": "yourserver.com",
"port": 1883,
"username": "youruser",
"passwd": "yourpass"
}
}
```

## Mqtt in Home Assistant
To use mqtt in Home Assistant, you have to setup a broker first:
```mqtt:
  broker: yourserver.com
  username: youruser 
  password: yourpass```
  
 After that, you can create a light with the mqtt platform. Only change values if you really know what you are doing :)
 ```light:
  - platform: mqtt
    name: "Wordclock"
    state_topic: "wordclock/status"
    command_topic: "wordclock/switch"
    rgb_command_topic: "wordclock/rgb/set"
    rgb_state_topic: "wordclock/rgb/status"
    brightness_command_topic: "wordclock/brightness/set"
    brightness_state_topic: "wordclock/brightness/status"
    rgb_command_template: "{{ '%03d%03d%03d' | format(red, green, blue)}}"
    qos: 2
    payload_on: "on"
    payload_off: "off"
    retain: false
    optimistic: false```