/***     Libraries    ***/
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncWiFiManager.h"  
#include "AsyncUDP.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

/**************************
 *    Variables           *
 **************************/
#define CURRENT A0  // Analog pin for nodeMCU
const char* ssid = "Mensah's Nokia";
const char* password = "lucille2";
/*
* BE sure to include the host address where we will be sending and receiving data 
* const char* host = "http://smarthub/address"
*/

+/+00 time LED was updated
const long interval = 5000;             // interval at which to send udp boadcast (milliseconds)

String ipaddress = "";
const char* udpMsg;
AsyncUDP udp;

WiFiClient client; // WiFi setup 


/****   Prototypes & Methods   ****/
AsyncWebServer server(80);       // Initialize server on port 80
DNSServer dns;        

void sendData(float current, float voltage);
float current();
float voltage();
void forwardData();
void connectToHub();
void switchDevice();

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

/********                              FUNCTIONS                           ************/
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
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host);
  
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  
  if (httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
  
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(root["sensor"].as<char*>());
    Serial.println(root["time"].as<char*>());
    Serial.println(root["data"][0].as<char*>());
    Serial.println(root["data"][1].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  
  delay(5000);  //GET Data at every 5 seconds
}

void sendData(float current, float voltage){
  // Creating json data
  StaticJsonBuffer<300> JSONbuffer; //Declaring static JSON buffer
  JsonObject &JSONencoder = JSONbuffer.createObject();

  //Encoding data
  JSONencoder["temperature"] = current;
  JSONencoder["level"] = voltage;

  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  // Declaring object class of  the HTTPClient
  HTTPClient http;

  http.begin(client, "http://192.168.205.185:8000/data/");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(JSONmessageBuffer); //Send the request
  String payload = http.getString();           //Get the response payload

  Serial.println(httpCode); //Print HTTP return code
  // if (httpCode == 200)
  // {
  //   for (int x = 0; x < 5; x++)
  //   {
  //     digitalWrite(GRN_LED, HIGH);
  //     delay(500);
  //     digitalWrite(GRN_LED, LOW);
  //     delay(500);
  //   }
  // }
  http.end(); //Close connection
}
