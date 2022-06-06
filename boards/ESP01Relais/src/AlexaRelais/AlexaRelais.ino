/**
   ESP01 Relais with Alexa

   For ESP8266/ESP32 with ESP01 and Relais Board
   Additionally you can switch on OTA


   Libs:
   Basis is the example form fauxmo https://bitbucket.org/xoseperez/fauxmoesp
   Additional Requirements:

   fouxmoESP https://bitbucket.org/xoseperez/fauxmoesp/src/master/

   ESP8266: ESPAsyncTCP  https://github.com/me-no-dev/ESPAsyncTCP
   ESP32:   AsyncTCP     https://github.com/me-no-dev/AsyncTCP

   For further information see also
   http://www.fambach.net if you want

   GPLv3
*/

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

// define your SSID and password for your WIFI network
//#define WIFI_SSID "TEST"
//#define WIFI_PASS "TEST123456789"

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE   115200
#define RELAY_PIN         0
#define RELAY_ID          "ESP Lampe"
#define LED_PIN           BUILTIN_LED
#define OTA_ENABLED       // enable over the air update


// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

#ifdef OTA_ENABLED
#include <ArduinoOTA.h>
#endif

void wifiSetup() {

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

  // Here starts the OTA configuration-----------------------------------------------------------------------------
#ifdef OTA_ENABLED
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.setHostname(RELAY_ID);
  ArduinoOTA.begin();

#endif
// OTA END-----------------------------------------------------------------------------



}

void setup() {

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();

  // LEDs
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Wifi
  wifiSetup();

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  fauxmo.enable(true);

  // Add virtual devices
  fauxmo.addDevice(RELAY_ID);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

    // Callback when a command from Alexa is received.
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

    // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
    // Otherwise comparing the device_name is safer.

    if (strcmp(device_name, RELAY_ID) == 0) {
      //digitalWrite(LED_BLUE, state ? HIGH : LOW);
      digitalWrite(RELAY_PIN, state ? HIGH : LOW);
      digitalWrite(LED_PIN, state ? LOW : HIGH);
    }
  });

}

void loop() {

#ifdef OTA_ENABLED
  ArduinoOTA.handle();
#endif

  // Handle events
  fauxmo.handle();
}
