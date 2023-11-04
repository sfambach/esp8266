/*
** 15.10.2023 debug oubput activated
** 


    This sketch shows how to use multiple WiFi networks.

    In this example, ESP8266 works in AP mode.
    It demonstrates:
    - Fast connect to previous WiFi network at startup
    - Registering multiple networks (at least 1)
    - Connect to WiFi with strongest signal (RSSI)
    - Fall back to connect to next WiFi when a connection failed or lost
    - Fall back to connect to hidden SSID's which are not reported by WiFi scan

    To enable debugging output, select in the Arduino iDE:
    - Tools | Debug Port: Serial
    - Tools | Debug Level: WiFi
*/

#define  DEBUG_ESP_WIFI
#define  DEBUG_WIFI
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

void setup() {
  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  Serial.println("\nESP8266 Multi WiFi example");
  Serial.println(WiFi.macAddress());

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  wifiMulti.addAP("NODES", "HappyNodes1234");
  wifiMulti.addAP("NODES2");
    // More is possible
}

void loop() {
  // Maintain WiFi connection
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi not connected!");
  }

  delay(1000);
}
