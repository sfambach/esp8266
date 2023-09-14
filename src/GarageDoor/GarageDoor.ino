/******************************************************************************
* Relay to give impulses to a door opener/closer
* MQTT send ms to defined topic to how long the relay should be on
* Licence: AGPL3
* Author: S. Fambach
* Website: http://Fambach.net
*******************************************************************************/
// SERIAL
#define BOUD 115200

// allow debug output
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif
/******************************************************************************/
// activate the service you need
#define MQTT_ACTIVE
#define WEB_ACTIVE
#define OTA_ACTIVE
                              


/******************************************************************************/
// WLAN
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#define WLAN_SSID    "NODES"
#define WLAN_PASSWD  "HappyNodes1234"

/******************************************************************************/
// Door
#define RELAIS_PIN 0
#define DEFAUL_DURATION_MS 200
long relayCurrentTS = 0;
long relayDelay = 0;
bool relayOn = false;

void relaySwitchOn(long ms){
  relayCurrentTS = millis();
  relayDelay = ms;
  relayOn = true;

  digitalWrite(RELAIS_PIN, HIGH);
}

void relayLoop(){
   if(relayOn){
      long elapsedTime = millis()-relayCurrentTS;
      if(elapsedTime >= relayDelay){
        digitalWrite(RELAIS_PIN, LOW);
        relayOn = false;
      }
   } 
}


/******************************************************************************/
// OTA
#ifdef OTA_ACTIVE
#include <ArduinoOTA.h>
#define  OTA_HOST_NAME "DoorKeeper"
#endif // OTA_ACTIVE
/******************************************************************************/
#ifdef MQTT_ACTIVE

#include <MQTTPubSubClient.h>

// MQTT
#define MQTT_CLIENTID "DoorKeeper"
#define MQTT_HOST_ADDRESS "192.168.10.102"
#define MQTT_HOST_PORT 1883
#define MQTT_USR "mqtt"
#define MQTT_PASSWD "Qwertz1234"
#define MQTT_TOPIC "/Haus/Garten/Nord/Hoftor"

WiFiClient client;
MQTTPubSubClient mqtt;

#endif // MQTT_ACTIVE
/******************************************************************************/
// Web server 

#ifdef WEB_ACTIVE
WiFiServer server(80);
String header;

void webLoop(){
  WiFiClient client = server.available();   // Hört auf Anfragen von Clients

  if (client) {                             // Falls sich ein neuer Client verbindet,
    Serial.println("Neuer Client.");          // Ausgabe auf den seriellen Monitor
    String currentLine = "";                // erstelle einen String mit den eingehenden Daten vom Client
    while (client.connected()) {            // wiederholen so lange der Client verbunden ist
      if (client.available()) {             // Fall ein Byte zum lesen da ist,
        char c = client.read();             // lese das Byte, und dann
        Serial.write(c);                    // gebe es auf dem seriellen Monitor aus
        header += c;
        if (c == '\n') {                    // wenn das Byte eine Neue-Zeile Char ist
          // wenn die aktuelle Zeile leer ist, kamen 2 in folge.
          // dies ist das Ende der HTTP-Anfrage vom Client, also senden wir eine Antwort:
          if (currentLine.length() == 0) {
            // HTTP-Header fangen immer mit einem Response-Code an (z.B. HTTP/1.1 200 OK)
            // gefolgt vom Content-Type damit der Client weiss was folgt, gefolgt von einer Leerzeile:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Hier werden die GPIO Pins ein- oder ausgeschaltet
            if (header.indexOf("GET /button/pressed") >= 0) {
              Serial.println("Button pressed");
              relaySwitchOn(DEFAUL_DURATION_MS);
            } 
            
            // Hier wird nun die HTML Seite angezeigt:
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            //client.println("<meta http-equiv=\"refresh\" content=\"1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Es folgen der CSS-Code um die Ein/Aus Buttons zu gestalten
            // Hier können Sie die Hintergrundfarge (background-color) und Schriftgröße (font-size) anpassen
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #333344; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #888899;}</style></head>");
            
            // Webseiten-Überschrift
            client.println("<body><h1>Door Opener</h1>");
            
            // Zeige den aktuellen Status, und AN/AUS Buttons for GPIO 5  
            client.println("<p>Relay - State " + String(relayOn) + "</p>");
            // wenn output5State = off, zeige den EIN Button       
          
            client.println("<p><a href=\"/button/pressed\"><button class=\"button\">PUSH</button></a></p>");
               
            client.println("</body></html>");
            
            // Die HTTP-Antwort wird mit einer Leerzeile beendet
            client.println();
            // und wir verlassen mit einem break die Schleife
            break;
          } else { // falls eine neue Zeile kommt, lösche die aktuelle Zeile
            currentLine = "";
          }
        } else if (c != '\r') {  // wenn etwas kommt was kein Zeilenumbruch ist,
          currentLine += c;      // füge es am Ende von currentLine an
        }
      }
    }
    // Die Header-Variable für den nächsten Durchlauf löschen
    header = "";
    // Die Verbindung schließen
    client.stop();
    Serial.println("Client getrennt.");
    Serial.println("");
  }
}
#endif // WEB_ACTIVE

/******************************************************************************/
#ifdef OTA_ACTIVE
void setupOTA(){
 // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(OTA_HOST_NAME);

  // No authentication by default
  // ArduinoOTA.setPassword("Gemuese76");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("ef84180df45cf2f22993e3a03dc71a27");
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    DEBUG_PRINTLN("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    DEBUG_PRINTLN("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      DEBUG_PRINTLN("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      DEBUG_PRINTLN("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      DEBUG_PRINTLN("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      DEBUG_PRINTLN("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      DEBUG_PRINTLN("End Failed");
    }
  });
  ArduinoOTA.begin();

}
#endif // OTA_ACTIVE

#ifdef MQTT_ACTIVE
void setupMQTT(){
    DEBUG_PRINTLN("connecting to host...");
    while (!client.connect(MQTT_HOST_ADDRESS, MQTT_HOST_PORT)) {
        DEBUG_PRINT(".");
        delay(1000);
    }
    DEBUG_PRINTLN(" connected!");

    // initialize mqtt client
    mqtt.begin(client);

    DEBUG_PRINTLN("connecting to mqtt broker...");
    while (!mqtt.connect(MQTT_CLIENTID, MQTT_USR, MQTT_PASSWD)) {
        DEBUG_PRINT(".");
        delay(1000);
    }
    DEBUG_PRINTLN(" connected!");

    // subscribe topic and callback which is called when /hello has come
    mqtt.subscribe(MQTT_TOPIC, [](const String& payload, const size_t size) {
        DEBUG_PRINT("Received: ");
        DEBUG_PRINTLN(payload);
        int ms = payload.toInt();
        relaySwitchOn(ms);
    });

}
#endif // MQTT_ACTIVE

void setup() {
  Serial.begin(BOUD);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASSWD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    DEBUG_PRINTLN("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();

  }

#ifdef OTA_ACTIVE
  setupOTA();
#endif // OTA_ACTIVE

#ifdef MQTT_ACTIVE
  setupMQTT();
#endif //MQTT_ACTIVE 

  DEBUG_PRINTLN("Ready");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());

  // init relais
  pinMode(RELAIS_PIN, OUTPUT);
  digitalWrite(RELAIS_PIN, LOW);

  // webserver
#ifdef WEB_ACTIVE
  server.begin();
#endif // WEB_ACTIVE

}

void loop() {

#ifdef OTA_ACTIVE
  // ota
  ArduinoOTA.handle();
#endif // OTA_ACTIVE

#ifdef MQTT_ACTIVE
  // mqtt
  mqtt.update();  
#endif // MQTT_ACTIVE

  relayLoop();

#ifdef WEB_ACTIVE
  // webinterface 
  webLoop();
#endif // WEB_ACTIVE

}
