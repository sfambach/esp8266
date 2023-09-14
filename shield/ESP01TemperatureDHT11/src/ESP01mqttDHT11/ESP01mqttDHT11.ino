/**
   ESP01 DHT 11 Board 
   
   Libs:
   https://github.com/knolleary/pubsubclient
   https://github.com/beegee-tokyo/DHTesp
   
   
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

// -----------------------------------------------------------------------------
// define your SSID and password for your WIFI network
#include "credentials.h"

// -----------------------------------------------------------------------------
// DHT
// -----------------------------------------------------------------------------
#include "DHTesp.h"
DHTesp dht;

// -----------------------------------------------------------------------------
// mqtt
// -----------------------------------------------------------------------------
#include "mqtt.h" // local file in the repository

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

  // Here starts the OTA configuration you don't have to care about it -------------------------------------
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

  // LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Wifi
  wifiSetup();

  // dht 
  dht.setup(PIN, SENSOR_TYPE);

  // mqtt 
  mqttSetup();
  

}


void startDeepSleep(){
  Serial.println("Going to deep sleep...");
  ESP.deepSleep(SLEEP_TIME);
  yield();
}

void loop() {

#ifdef OTA_ENABLED
  ArduinoOTA.handle();
#endif


  // DHT handling
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toCelsius(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toCelsius(temperature), humidity, true), 1);

  mqttPublish(TOPIC +"/" + TEMP_CAT,  temperature);
  mqttPublish(TOPIC +"/" + HUM_CAT,   humidity;
  char stringBuffer80[80]; 
  spritnf(stringBuffer80,JSON_TOPIC_TEMPLATE , String(temperature), String(humidity));
  mqttPublish(TOPIC +"/" + JSON_CAT, stringBuffer80 );

  mqttLoop();

  
}
