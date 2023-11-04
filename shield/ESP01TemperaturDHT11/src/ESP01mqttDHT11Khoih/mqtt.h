#ifndef MY_MQTT_H
#define MY_MQTT_H
#include "debug.h"
#include "credentials.h"
#include <MQTTPubSubClient_Generic.h> //https://github.com/khoih-prog/MQTTPubSubClient_Generic/tree/main

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>

#endif

WiFiClient client;
MQTTPubSubClient mqttClient;
//void receivedCallback(char* topic, byte* payload, unsigned int length);



//PubSubClient mqttClient(BROKER,BRPORT,espClient);

void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  DEBUG_PRINT("[MQTT] Message received: ");
  DEBUG_PRINTLN(topic);

  DEBUG_PRINT("[MQTT] payload: ");
  for (int i = 0; i < length; i++) DEBUG_PRINT((char)payload[i]);
  DEBUG_PRINTLN();
}



boolean mqttConnected(){
  boolean con = mqttClient.isConnected();
 // DEBUG_PRINTLN(String(" Connected=")+String(con?"TRUE":"FALSE"));
  return con;
}

void mqttConnect() {
  DEBUG_PRINT("[MQTT] Check connection ...");
  /* Loop until reconnected */
  while (!mqttClient.connect(CLIENTID, BRUSER, BRPWD))
  {
    DEBUG_PRINT(".");
    delay(1000);
  }
  DEBUG_PRINTLN(" OK");
}

void mqttSetup()
{
  DEBUG_PRINT("Connect to server ");
  while (!client.connect(BROKER, BRPORT))
  {
    DEBUG_PRINT(".");
    delay(1000);
  }
  DEBUG_PRINTLN(" OK");
  mqttClient.begin(client);
  mqttConnect();


}

void mqttPublish(const char* topic, const char* content){
  DEBUG_PRINT("[MQTT] send str   message  \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  
  mqttClient.publish(topic, content, true, 1);  
}
void mqttPublish(const char* topic, int32_t content){
  DEBUG_PRINT("[MQTT] send int   message  \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  mqttClient.publish(topic, String(content).c_str(), true, 1);  
}


void mqttPublish(const char* topic, float content){
  DEBUG_PRINT("[MQTT] send float message \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  mqttClient.publish(topic, String(content).c_str(), true, 1);  
}


void mqttDisconnect(){
  DEBUG_PRINTLN("[MQTT] Disconnect");
  mqttClient.disconnect();
}

void mqttLoop(){
  if(!mqttConnected()){
    mqttConnect();
  }
  mqttClient.update();
}
#endif // #define MY_MQTT_H
