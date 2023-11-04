/**
   ESP01 DHT 11 Board 
   
   Libs:
   https://github.com/knolleary/pubsubclient
   https://github.com/beegee-tokyo/DHTesp
   https://github.com/matmunk/DS18B20
   
   
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

#if defined(SENSOR_DHT11)
#include "DHTesp.h"
DHTesp dht;
DHTesp::DHT_MODEL_t model = DHTesp::DHT_MODEL_t::DHT11;
void setupSensor() {
  dht.setup(17, model);
}

#elif defined(SENSOR_DHT22)
#include "DHTesp.h"
DHTesp dht;
DHTesp::DHT_MODEL_t model = DHTesp::DHT_MODEL_t::DHT22;
void setupSensor() {
  dht.setup(17, model);
}

#elif defined(SENSOR_18BS20)
#include <DS18B20.h>
DS18B20 ds(2);
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

  ArduinoOTA.setHostname(RELAY_ID);
  ArduinoOTA.begin();

#endif
  // OTA END-----------------------------------------------------------------------------
}

void printMeasurement() {

#if defined(SENSOR_DHT11) || defined(SENSOR_DHT22)
  delay(dht.getMinimumSamplingPeriod());
  humidity = dht.getHumidity();
  temperature = dht.toCelsius(dht.getTemperature());

#elif define(SENSOR_18BS20)
  if (selected) {
    humidity = -1;
    temperature = ds.getTempC());
  }


#else
#error "Wrong sensor"
#endif

  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.println(temperature, 1);
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

  // Sensor
  setupSensor();

// mqtt
#ifdef MQTT_ENABLED
  mqttSetup();
#endif
}


void startDeepSleep() {
  Serial.println("Going to deep sleep...");
  ESP.deepSleep(SLEEP_TIME);
  yield();
}

void loop() {

#ifdef OTA_ENABLED
  ArduinoOTA.handle();
#endif


  // DHT handling
  printMeasurement();


#ifdef MQTT_ENABLED
  if(!mqttConnected()){
      mqttConnect();
  }
  
  String cat;
  cat.reserve(400);

  cat = String(TOPIC) + String("/") + String(TEMP_CAT);
  mqttPublish(cat.c_str(), temperature);
delay(200);
  cat = String(TOPIC) + String("/") + String(HUM_CAT);
  mqttPublish(cat.c_str(), humidity);
delay(200);
  char stringBuffer80[80];
  sprintf(stringBuffer80, JSON_TOPIC_TEMPLATE, String(temperature), String(humidity));
  cat = String(TOPIC) + String("/") + String(JSON_CAT);
  mqttPublish(cat.c_str(), stringBuffer80);

//mqttLoop();

#endif  //MQTT_ENABLED

  //delay(2000);  // only for test purpose
  startDeepSleep();
}
