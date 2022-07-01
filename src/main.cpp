#include "Functions.h"
#include <Arduino.h>
#include "painlessMesh.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>
#include <Arduino_JSON.h>
using namespace std;

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SERVER_PIN, OUTPUT);
  pinMode(WIFI_PIN, OUTPUT);
  initMesh();
  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }
}

void loop()
{
  mesh.update();
  server.handleClient();
  MDNS.update();
  // getWiFiConnectionStatus();
}