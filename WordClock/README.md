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
}
}
```
