/*
  Create an Accesspoint
   Inspired by Majenko Technologies example scetch
  Author: SteFam
  Web: http://www.fambach.net
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SSID "123espAP_1"
#define PASSWD  "lETmEiN123456"
const char * ssid = SSID;
const char* passwd = PASSWD;
ESP8266WebServer server(8080);


void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void initAP(){
  Serial.print("Setup Accesspoint .....");
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(true);
  WiFi.encryptionType(ENC_TYPE_TKIP);
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  WiFi.setHostname("HUND");
  WiFi.mode(WIFI_AP);
  WiFi.setAutoConnect(false);
  WiFi.softAP(ssid, passwd , 3, 0, 4);// channel // hide ssid // max connections 
  delay(1000);
  

  WiFi.printDiag(Serial);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.print(ip);
  Serial.print( "... SSID: ");
  Serial.print(WiFi.softAPSSID());


  Serial.println(".....done");
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  digitalWrite(2, LOW);
  initAP();

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();
}
