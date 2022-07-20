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
  pinMode(RELAY_PIN, INPUT);
  pinMode(AMBER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  if (isRoot)
  {
    pinMode(CURRENT, OUTPUT);
    pinMode(VOLTAGE, OUTPUT);
    digitalWrite(VOLTAGE, HIGH);
    digitalWrite(CURRENT, LOW);
  }
  else
  {
    pinMode(CURRENT, OUTPUT);
    pinMode(VOLTAGE, OUTPUT);
    digitalWrite(VOLTAGE, LOW);
    digitalWrite(CURRENT, HIGH);
    // digitalWrite(RELAY_PIN, LOW);
  }
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