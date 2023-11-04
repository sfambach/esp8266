/**
   ESP01 DHT 11 Board 
   
   Libs:
   https://github.com/khoih-prog/MQTTPubSubClient_Generic
   https://github.com/adafruit/DHT-sensor-library
   https://github.com/beegee-tokyo/DHTesp
   https://github.com/matmunk/DS18B20
   https://github.com/arduino/arduinoOTA
   
   
   For further information see also
   http://www.fambach.net if you want

   GPLv3
*/

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY !)
#error Select ESP8266 board.
#else
#include <ESP8266WiFi.h>
#endif

// -----------------------------------------------------------------------------
// define your SSID and password for your WIFI network
#include "credentials.h"

// -----------------------------------------------------------------------------
// Sensors
// -----------------------------------------------------------------------------
float humidity;
float temperature;
String status;

#if defined(SENSOR_DHT11)
//#include "DHTesp.h"
//DHTesp dht;

#include <DHT.h>
DHT dht(SENSOR_PIN, DHT11);

void setupSensor() {
  //dht.setup(SENSOR_PIN, DHTesp::DHT11);
  dht.begin();
}

#elif defined(SENSOR_DHT22)
#include "DHTesp.h"
DHTesp dht;
void setupSensor() {
  dht.setup(SENSOR_PIN, DHTesp::DHT22);
}

#elif defined(SENSOR_18BS20)
#include <DS18B20.h>
DS18B20 ds(SENSOR_PIN);
uint8_t address[] = { 40, 250, 31, 218, 4, 0, 0, 52 };
uint8_t selected;

void setupSensor() {
  selected = ds.select(address);
  if (!selected) {
    Serial.println("Device not found!");
    // show all devices
    while (ds.selectNext()) {
      switch (ds.getFamilyCode()) {
        case MODEL_DS18S20:
          getAddress(address);
          Serial.println(address)
            Serial.println("Model: DS18S20/DS1820");
          break;
        case MODEL_DS1822:
          Serial.println("Model: DS1822");
          break;
        case MODEL_DS18B20:
          Serial.println("Model: DS18B20");
          break;
        default:
          Serial.println("Unrecognized Device");
          break;
      }
    }
  }

#else
#error "Unknown Sensor please select DHT11 / DHT22 / 18BS20"

#endif  // Sensors


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
    else  // U_SPIFFS
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

  ArduinoOTA.setHostname(String(RELAY_ID).c_str());
  ArduinoOTA.begin();

#endif
  // OTA END-----------------------------------------------------------------------------
}

boolean printMeasurement() {

#if defined(SENSOR_DHT11) || defined(SENSOR_DHT22)
/*  delay(dht.getMinimumSamplingPeriod());
  humidity = NAN;
  while (isnan(humidity)) {
    humidity = dht.getHumidity();
  }
  temperature = dht.getTemperature();
  status = dht.getStatusString();
*/
 
 
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

#elif define(SENSOR_18BS20)
  if (selected) {
    humidity = -1;
    temperature = ds.getTempC());
    staus = "";
  }


#else
#error "Wrong sensor"
#endif
  DEBUG_PRINTLN(status);
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.println(temperature, 1);
  return true;
}



void setup() {

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);

  // LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Wifi
  wifiSetup();

  // Sensor
  setupSensor();

// mqtt
#ifdef MQTT_ENABLED
  mqttSetup();
#endif
}


void startDeepSleep() {
  DEBUG_PRINTLN("Going to deep sleep...");
  ESP.deepSleep(SLEEP_TIME);
  yield();
}

long curTime;

void loop() {

#ifdef OTA_ENABLED
  ArduinoOTA.handle();
#endif

#ifndef DEEP_SLEEP_ENABLED
  // check non blocking time interval
  if (millis() > curTime + MEASURMENT_INTERVAL_MS) {
#endif
    // DHT handling
    if(!printMeasurement()){return;}

#ifdef OTA_ENABLED
    ArduinoOTA.handle();
#endif

#ifdef MQTT_ENABLED
    if (!mqttConnected()) {
      ESP.reset();
    }

    String cat;
    cat.reserve(400);
#ifdef OTA_ENABLED
    ArduinoOTA.handle();
#endif
    cat = String(TOPIC) + String("/") + String(TEMP_CAT);
    mqttPublish(cat.c_str(), temperature);
//delay(200);
#ifdef OTA_ENABLED
    ArduinoOTA.handle();
#endif
    cat = String(TOPIC) + String("/") + String(HUM_CAT);
    mqttPublish(cat.c_str(), humidity);
//delay(200);
#ifdef OTA_ENABLED
    ArduinoOTA.handle();
#endif

    cat = String(TOPIC) + String("/") + String(STATE_CAT);
    mqttPublish(cat.c_str(), status.c_str());
//delay(200);
#ifdef OTA_ENABLED
    ArduinoOTA.handle();
#endif
    char stringBuffer80[80];
    sprintf(stringBuffer80, JSON_TOPIC_TEMPLATE, String(temperature), String(humidity), status);
    cat = String(TOPIC) + String("/") + String(JSON_CAT);
    mqttPublish(cat.c_str(), stringBuffer80);

#endif  //MQTT_ENABLED
#ifndef DEEP_SLEEP_ENABLED
    curTime = millis();
  }  // close time interval check
#else
  startDeepSleep();
#endif
}
