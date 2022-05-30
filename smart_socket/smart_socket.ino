/***     Libraries    ***/
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncWiFiManager.h"  
#include "AsyncUDP.h"

/**************************
 *    Variables           *
 **************************/
#define CURRENT A0  // Analog pin for nodeMCU
const char* ssid = "Mensah's Nokia";
const char* password = "lucille2";

/****   Prototypes &    ****/

void setup() {
  // put your setup code here, to run once:
  pinMode(CURRENT, INPUT);   // setting A0 as read
}

void loop() {
  // put your main code here, to run repeatedly:
  
}


// Measuring Current
float current() {
  float resolution = 3.3 / 1024; // Input Voltage Range is 1V to 3.3V
                                 // ESP8266 ADC resolution is 10-bit. 2^10 = 1024
  float rawCurrent = analogRead(CURRENT);
  float actCurrent = rawCurrent * resolution;
  Serial.print(actCurrent, 3);
  return actCurrent;
}

float voltage() {
  // Voltage measuring code here
  return;
}

void forwardData() {
 // code to forward current and voltage to smart hub
}

void connectToHub() {
  // code to connect to smart hub network
}

void switchDevice() {
  // code to turn off/on device remotely
}
