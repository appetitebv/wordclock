#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(80);
IPAddress Ip(192, 168, 1, 1);
IPAddress NMask(255, 255, 255, 0);
  
const char* ssid = "Wordclock Wi-Fi";
const char* passphrase = "test1234";
String st;
String content;
int statusCode;

void setup() {
   // initialize serial and wait for port to open:
  Serial.begin(115200);
  EEPROM.begin(512);

  // Get ssid from eeprom
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid);

  // Get pass from eeprom
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i) {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi()) {
    Serial.println("Wifi available");
    return;
  }

  setupAP();
}

void loop() {
  server.handleClient();
}

void writeProm(String qsid, String qpass) {        
  Serial.println("clearing eeprom");
  for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }

  Serial.println("writing eeprom ssid:");
  for (int i = 0; i < qsid.length(); ++i) {
    EEPROM.write(i, qsid[i]);
  }
  
  Serial.println("writing eeprom pass:"); 
  for (int i = 0; i < qpass.length(); ++i) {
    EEPROM.write(32+i, qpass[i]);
  }    
  EEPROM.commit();
  Serial.println("saved to eeprom... reset to boot into new wifi");
}

bool testWifi() {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { 
      return true;
    } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
} 

void setupAP() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
     }
  }
  Serial.println(""); 
  st = "<ol>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ol>";
  delay(100);

  WiFi.softAPConfig(Ip, Ip, NMask);
  WiFi.softAP(ssid, passphrase, 6);
  launchWeb();
}

void launchWeb() {
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}

void createWebServer()
{
  server.on("/", []() {
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);  
  });
  server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
          
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
          {
            EEPROM.write(i, qsid[i]);
            Serial.print("Wrote: ");
            Serial.println(qsid[i]); 
          }
        Serial.println("writing eeprom pass:"); 
        for (int i = 0; i < qpass.length(); ++i)
          {
            EEPROM.write(32+i, qpass[i]);
            Serial.print("Wrote: ");
            Serial.println(qpass[i]); 
          }    
        EEPROM.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.send(statusCode, "application/json", content);
  });
}
