
#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// -----------------------------------------------------------------------------
// define your SSID and password for your WIFI network

#define WIFI_SSID "NODES"
#define WIFI_PASS "HappyNodes1234"

#define RELAY_ID "ESPDHT11"
#define SERIAL_BAUDRATE 115200
#define PIN 0
//#define SLEEP_TIME 5 * 60000  // 5 minutes
#define SLEEP_TIME 1 * 2000  // 2 sec
#define LED_PIN BUILTIN_LED

#define OTA_ENABLED  // enable over the air update
#define MQTT_ENABLED

// -----------------------------------------------------------------------------
// Sensor
// -----------------------------------------------------------------------------

// select your sensor
#define SENSOR_DHT11
//#define SENSOR_DHT22
//#define SENSOR_18BS20


// -----------------------------------------------------------------------------
// mqtt
// -----------------------------------------------------------------------------
#ifdef MQTT_ENABLED
const char* BROKER = "192.168.10.102";
uint16_t BRPORT = 1883;
#define BRUSER "mqtt"
#define BRPWD "Qwertz1234"
#define CLIENTID "TEMPSensor"
#include "mqtt.h"  // local file in the repository

#define TOPIC "Haus/Zimmer/Temp"
#define HUM_CAT "Humidity"
#define TEMP_CAT "Temperature"
#define JSON_CAT "json"
#define JSON_TOPIC_TEMPLATE "[{\"Temperature\": \"%s\",\"Humidity\":\"%s\"}]"

#endif  // MQTT_ENABLED
#endif
