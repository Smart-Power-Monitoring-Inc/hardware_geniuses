#include "Functions.h"
#include <Arduino.h>
#include "painlessMesh.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>
#include <Arduino_JSON.h>

using namespace std;

// #define MESH_PREFIX "whateverYouLike"   // mesh prefix
// #define MESH_PASSWORD "somethingSneaky" // mesh password
// #define MESH_PORT 5555
// #define SSID "Mensah's Nokia"               // smart hub SSID
// // #define SSID "keep-project"               // smart hub SSID
// // #define PASS "NgTUiPk8"                   // smart hub password
// #define PASS "lucille2"                   // smart hub password
// #define RELAY_PIN D1                      // relay
// #define SERVER_PIN D2                     // relay
// #define WIFI_PIN D3                       // relay
// #define NODE_ID ASCIISentence(deviceName) // relay
// WiFiClient client;
// HTTPClient http;
// unsigned long previousMillis = 0;
// unsigned long interval = 5000;
// // Function Prototypes
// void connectToWiFi();
// int ASCIISentence(String str);
// void errorBlin_NO_CONNECTION();
// void sendMessage(); // Prototype so PlatformIO doesn't complain
// Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
// void sendMessageToNode(u_int32_t nodeId);
// String getCurrentReading();
// void switchOffNodeRelay();
// void postData(String data);
// void errorBlink();
// void WIFI_CONNECTED(bool state);
// // variables - constants
// const char *ssid = SSID;
// const char *password = PASS;
// String deviceName = "Bless"; // Set this value for each smart socket. THeir names are under the esp82366
// // String deviceName = "Root Node"; // Set this value for each smart socket. THeir names are under the esp82366
// bool wifiConnected = false;
// Scheduler userScheduler; // to control your personal task
// painlessMesh mesh;
// // DynamicJsonDocument doc(1024);
// DynamicJsonDocument staticDoc(1024);
// bool toggle = false; // pin status of node's relay
// // bool wifiConnected = false;
// WiFiEventHandler wifiConnectHandler;
// WiFiEventHandler wifiDisconnectHandler;

//////////////////////////////////////////////////////
///             SMART HUB ONLY                    ///

/// Valid comands
// switch - toggle on or off
// node = send message to a node
// error = send error to nodes
//
// int ASCIISentence(String str)
// {
//   // Allows each node to have a unique id based on the ASCII value of its name
//   int l = str.length();
//   int convert = 0;
//   for (int i = 0; i < l; i++)
//   {
//     convert = str[i] - NULL;
//   }
//   return convert;
// }

// This should only be for node 1
// Only call this function if node id = 13194096279 - node 1's id
// void connectToWiFi()
// {
//   WiFi.disconnect();
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password); // Connect to the network
//   int i = 0;

//   while (WiFi.status() != WL_CONNECTED)
//   { // Wait for the Wi-Fi to connect
//     delay(1000);
//     i++;
//     WIFI_CONNECTED(false);
//     if (i == 100)
//     {
//       errorBlin_NO_CONNECTION();
//       break;
//     }
//     Serial.print('.');
//     errorBlink(); // blink when wifi is not connected
//   }
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     WIFI_CONNECTED(true);
//   }

//   Serial.print("IP address:\t");
//   Serial.println(WiFi.localIP());
//    WiFi.setAutoReconnect(true);
//   WiFi.persistent(true);
// }

// void postData(String data)
// {
//   // WiFiClient client;
//   // HTTPClient http;
//   if ((WiFi.status() == WL_CONNECTED))
//   {
//     WiFiClient client;
//     HTTPClient http;
//     // const char *host = "http://10.42.0.1:3000/summary/current"; // edit the host adress, ip address etc.
//     const char *host = "https://api.freerealapi.com/comments/618a862db883dbaafa9449c5"; // edit the host adress, ip address etc.
//     if (http.begin(client, host))
//     {

//       // int httpResponseCode = http.POST(""); // endpoint for posting data
//       http.addHeader("Content-Type", "application/json");
//       // Used to post data to the server
//       // Post received data to server
//       // Only used by node 1
//       int httpResponseCode = http.GET();
//       Serial.print("HTTP Response Code ");
//       Serial.println(httpResponseCode);
//       Serial.println(http.getString());
//     }
//     else
//     {
//       Serial.println("HTTP connection failed");
//     }
//     http.end();
//   }
// }

// void sendMessageToNode(int nodeId)
// {
//   // to be used by node 1 only
//   // Used to send a message to a particular node
//   // send message to the node with the correct id
//   if (nodeId == NODE_ID)
//   {
//     // parse the message here
//     // either send a command to relay
//     // or trigger error lights
//     String msg = "";              // json data to particular node
//     mesh.sendSingle(nodeId, msg); // should send message to a particular node
//   }
// }

// void WIFI_CONNECTED(bool state)
// {
//   if (state)
//   {
//     digitalWrite(WIFI_PIN, HIGH);
//   }
//   else
//   {
//     digitalWrite(WIFI_PIN, LOW);
//   }
// }

// ////////////////////////////////////////////////
// String getCurrentReading()
// {
//   JSONVar doc;
//   String json = "";
//   doc["id"] = NODE_ID;
//   doc["current"] = analogRead(A0);
//   // doc["time"] = millis();
//   doc["name"] = deviceName;
//   doc["command"] = "";
//   json = JSON.stringify(doc);

//   // Not on node 1
//   // Used to get the current readings
//   // Only used every other node except node 1
//   // return current readings
//   return json;
// }

// // Not on node 1
// void switchOffNodeRelay()
// {
//   // Swtich of the relay of the node if the command is received
//   // command - switch
//   // parse on or off command

//   if (toggle)
//   {
//     digitalWrite(RELAY_PIN, HIGH);
//     toggle = !toggle;
//   }
//   else
//   {
//     digitalWrite(RELAY_PIN, LOW);
//     toggle = !toggle;
//   }
// }

// void sendMessage()
// {
//   // to be used by the nodes
//   // serializeJson( getCurrentReading());
//   String msg = getCurrentReading();
//   mesh.sendBroadcast(msg);

//   taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
// }

// Needed for painless library
// void receivedCallback(uint32_t from, String &msg)
// {
//   // check if the incoming message contains a nodeid
//   // if the id matches the id of the node, parse the data
//   // %u short unsigned interget
//   //  %s String
//   Serial.printf("Received from %u msg=%s\n ", from, msg.c_str());
//   // parse data here
//   JSONVar parseJson = JSON.parse(msg.c_str());
//   int nodeId = parseJson["id"];               // short node id
//   const char *command = parseJson["command"]; // command to be issued to the node
//   const char *current = parseJson["current"]; // node current
//   const char *name = parseJson["name"];       // node name
//                                               /*
//                                               If command == switch
//                                               toggle on/off the node's relay
//                                               if command  == node
//                                               error = node should parse the error command
//                                               */
//   if (command == "switch" && nodeId == NODE_ID)
//   {
//     // ensure the correct node toggles switch
//     switchOffNodeRelay();
//   }
//   if (command == "error")
//   {
//     // call error functions
//     errorBlink();
//   }
//   // only for node 1
//   postData("sdfsdfsdf");
// }
// void errorBlink()
// {
//   // do something here
//   digitalWrite(SERVER_PIN, HIGH);
//   delay(500);
//   digitalWrite(SERVER_PIN, LOW);
//   delay(200);
// }
// void errorBlin_NO_CONNECTION()
// {
//   // do something here
//   while (true)
//   {
//     digitalWrite(SERVER_PIN, HIGH);
//     delay(200);
//     digitalWrite(SERVER_PIN, LOW);
//     delay(200);
//   }
// }
// void newConnectionCallback(uint32_t nodeId)
// {
//   // %u short unsigned interget
//   //  %s String
//   Serial.printf("--> New Connection, nodeId = %u\n", nodeId);
// }

// void changedConnectionCallback()
// {
//   Serial.printf("Changed connections\n");
// }

// void nodeTimeAdjustedCallback(int32_t offset)
// {
//   Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
// }

// void onWiFiConnect(const WiFiEventStationModeGotIP& event){
//   Serial.print("Connected to WiFi successfully");
// }

// void onWiFiDisconnect(const WiFiEventSoftAPModeStationDisconnected& event){
//   Serial.println("trying to connect");
// // ESP.restart();
//   // WiFi.disconnect();
//   // WiFi.begin(ssid,password);
//   // WiFi.reconnect();
// }

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SERVER_PIN, OUTPUT);
  pinMode(WIFI_PIN, OUTPUT);
  /////////////////////////
  connectToWiFi();
  initMesh();
}

void loop()
{
  // it will run the user scheduler as well
  mesh.update();
  getWiFiConnectionStatus();
}