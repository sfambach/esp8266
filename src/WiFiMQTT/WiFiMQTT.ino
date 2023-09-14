
#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <MQTTPubSubClient.h>

const char* ssid = "NODES";
const char* pass = "HappyNodes1234";

WiFiClient client;
MQTTPubSubClient mqtt;

#define MQTT_CLIENTID "KeyMan"
#define MQTT_USR "mqtt"
#define MQTT_PASSWD "Qwertz1234"

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    Serial.print("connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");

    Serial.print("connecting to host...");
    while (!client.connect("192.168.10.102", 1883)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");

    // initialize mqtt client
    mqtt.begin(client);

    Serial.print("connecting to mqtt broker...");
    while (!mqtt.connect(MQTT_CLIENTID, MQTT_USR, MQTT_PASSWD)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");

    // subscribe callback which is called when every packet has come
    mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        Serial.println("mqtt received: " + topic + " - " + payload);
    });

    // subscribe topic and callback which is called when /hello has come
    mqtt.subscribe("/Haus", [](const String& payload, const size_t size) {
        Serial.print("/Haus ");
        Serial.println(payload);
    });
}

void loop() {
    mqtt.update();  // should be called

    // publish message
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 1000) {
        prev_ms = millis();
        //mqtt.publish("/hello", "world");
    }
}
