#include <Arduino.h>
#include "painlessMesh.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 


#define MESH_PREFIX "whateverYouLike" // mesh prefix
#define MESH_PASSWORD "somethingSneaky" // mesh password
#define MESH_PORT 5555
#define SSID "keep-project" // smart hub SSID
#define PASS "NgTUiPk8" // smart hub password

// Function Prototypes
void connectToWiFi();
void setupHotSpot();
void sendMessage(); // Prototype so PlatformIO doesn't complain
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
void sendMessageToNode();
String getCurrentReading();
void switchOffNodeRelay();
void postData(String data);

// variables - constants
const char *ssid = SSID;
const char *password = PASS;
String deviceName = "Bless"; // Set this value for each smart socket. THeir names are under the esp82366
bool wifiConnected = false;
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
DynamicJsonDocument doc(4096);


//////////////////////////////////////////////////////
///             SMART HUB ONLY                    ///

// This should only be for node 1
// Only call this function if node id = 13194096279 - node 1's id
void connectToWiFi()
{
   WiFiClient client;
  HTTPClient http;
  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }
const char* host = "SMART-HUB'S IP:3000"; //edit the host adress, ip address etc. 
 
  http.begin(client,host);  
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

void postData(String data)
{
   WiFiClient client;
  HTTPClient http;
const char* host = "SMART-HUB'S IP:3000"; //edit the host adress, ip address etc. 
  http.begin(client,host);  

  int httpResponseCode = http.POST("/api/data"); // endpoint for posting data
  http.addHeader("Content-Type", "application/json");
  // Used to post data to the server
  // Post received data to server
  // Only used by node 1
   
}

void sendMessageToNode()
{
  // Used to send a message to a particular node
}

////////////////////////////////////////////////
String getCurrentReading()
{
  doc["id"] = mesh.getNodeId();
  doc["current"] = analogRead(A0);
  doc["time"] = millis();
  doc["name"] = deviceName;
  TSTRING json = "";

  serializeJson(doc, json );
  // Not on node 1
  // Used to get the current readings
  // Only used every other node except node 1
  // return current readings
  return json;
}

// Not on node 1

// Not on node 1
void switchOffNodeRelay()
{
  // Swtich of the relay of the node if the command is received
}


void sendMessage()
{
  // to be used by the nodes
  // serializeJson( getCurrentReading());
  String msg =  getCurrentReading();
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

// Needed for painless library
void receivedCallback(uint32_t from, String &msg)
{
  // check if the incoming message contains a nodeid
  // if the id matches the id of the node, parse the data
  Serial.printf("startHere: Received from %u msg=%s\n ", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}



void setup()
{
  Serial.begin(115200);
  connectToWiFi(); // Comment this out if uploading to other nodes. Only use this for node 1
  //  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop()
{
  // it will run the user scheduler as well
  mesh.update();
}