#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "OTAUpdate.h"
#include "WebConfig.h"

#ifndef STASSID
#define STASSID "DM"
#define STAPSK  "kamardimana"
#endif

const char* host = "esp32-webupdate";
const char* ssid = "DM";
const char* password = "kamardimana";


void setup(void){
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/led_hidup", [](){
    server.send(200, "text/plain", "LED Hidup");
    Serial.println();
    Serial.println("LED Hidup");
    digitalWrite(led, HIGH);
  });

  server.on("/led_mati", [](){
    server.send(200, "text/plain", "LED Padam");
    Serial.println();
    Serial.println("LED Padam");
    digitalWrite(led, LOW);
  });

  server.onNotFound(handleNotFound);
  OTA_Update();
  
  if (MDNS.begin("esp32-cyberfreak")) {
    Serial.println("MDNS responder started");
  }
  server.begin();
  Serial.println("HTTP server started");

  for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	};

  httpUpdater.setup(&server);
  server.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);

}

void loop(void){
  server.handleClient();
  ArduinoOTA.handle();
}
