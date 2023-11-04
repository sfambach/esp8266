
#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// -----------------------------------------------------------------------------
// define your SSID and password for your WIFI network

#define WIFI_SSID "NODES"
#define WIFI_PASS "HappyNodes1234"

#define RELAY_ID "ESPDHT11"
#define SERIAL_BAUDRATE 115200

#define OTA_ENABLED  // enable over the air update
#define MQTT_ENABLED
//#define DEEP_SLEEP_ENABLED


// -----------------------------------------------------------------------------
// Deep Sleep
// -----------------------------------------------------------------------------
//#define SLEEP_TIME 5 * 60000000  // 5 minutes
#define SLEEP_TIME 10  // 10 micro sec
#define LED_PIN BUILTIN_LED

// if no deep sleep give mesurement interval.
#define MEASURMENT_INTERVAL_S 20 // meausure every 20 seconds
#define MEASURMENT_INTERVAL_MS ( MEASURMENT_INTERVAL_S * 1000)

// -----------------------------------------------------------------------------
// Sensor
// -----------------------------------------------------------------------------

// select your sensor
#define SENSOR_DHT11
//#define SENSOR_DHT22
//#define SENSOR_18BS20

#define SENSOR_PIN 2


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

#define TOPIC "Haus/Zimmer/DHT11"
#define HUM_CAT "Humidity"
#define TEMP_CAT "Temperature"
#define STATE_CAT "State"
#define JSON_CAT "json"
#define JSON_TOPIC_TEMPLATE "[{\"Temperature\": \"%s\",\"Humidity\":\"%s\",\"State\":\"%s\"}]"

#endif  // MQTT_ENABLED
#endif
