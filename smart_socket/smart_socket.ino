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

unsigned long previousMillis = 0;       // will store last time LED was updated
const long interval = 5000;             // interval at which to send udp boadcast (milliseconds)

String ipaddress = "";
const char* udpMsg;
AsyncUDP udp;

/****   Prototypes & Methods   ****/
AsyncWebServer server(80);       // Initialize server on port 80
DNSServer dns;        

void setup() {
  // put your setup code here, to run once:
  pinMode(CURRENT, INPUT);   // setting A0 as read

  Serial.begin(115200);       // Start serial ports

  //Try to connect to WIFI if not, start as AP.
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Connected to WIFI!");

  //Once connected to WIFI, set up and launch webserver.
  // AsyncServerSetup();
  server.begin();

  //Print IP address to terminal.
  ipaddress =  "HUB|"+ WiFi.localIP().toString();
  Serial.println();
  Serial.println(ipaddress);
  Serial.println("Listening...");

  //Connect to UDP Port.
  udp.connect(IPAddress(192,168,1,255), 19700);
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
  float actVoltage;
  return actVoltage;
}

void forwardData() {
 // code to forward current and voltage to smart hub
}

void connectToHub() {
  // code to connect to smart hub network
  unsigned long currentMillis = millis();
  // CheckRadio();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    udp.broadcastTo(ipaddress.c_str(), 19700);
  }
}

void switchDevice() {
  // code to turn off/on device remotely
}
