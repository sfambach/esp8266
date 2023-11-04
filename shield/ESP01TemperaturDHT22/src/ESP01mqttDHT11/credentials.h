
#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// -----------------------------------------------------------------------------
// define your SSID and password for your WIFI network
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"

#define RELAY_ID "ESPDHT11"
#define SERIAL_BAUDRATE   115200
#define PIN               0
#define SLEEP_TIME        1*60000 // 5 minutes
#define LED_PIN           BUILTIN_LED
#define OTA_ENABLED       // enable over the air update

// -----------------------------------------------------------------------------
// DHT
// -----------------------------------------------------------------------------
#include "DHTesp.h"
DHTesp dht;

#define SENSOR_TYPE DHTesp::DHT11
//#define SENSOR_TYPE DHTesp::DHT22

// -----------------------------------------------------------------------------
// mqtt
// -----------------------------------------------------------------------------
#include "mqtt.h" // local file in the repository
const char* BROKER = "192.168.1.10";
uint16_t    BRPORT = 1883;
const char* BRUSER = "admin";
const char* BRPWD  = "admin";
const char* CLIENTID = "ESP01DHT11";

#define TOPIC  "ESP01/DHT11/FIRST" 
#define HUM_CAT "Humidity"
#define TEMP_CAT "Temperature"
#define JSON_CAT "json"
#define JSON_TOPIC_TEMPLATE  "[{\"Temperature\": \"%s\",\"Humidity\":\"%s\"}]"

#endif
