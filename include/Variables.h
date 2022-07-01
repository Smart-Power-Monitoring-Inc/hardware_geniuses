
#include "Libraries.h"
using namespace std;
#define MESH_PREFIX "whateverYouLike"   // mesh prefix
#define MESH_PASSWORD "somethingSneaky" // mesh password
#define MESH_PORT 5555
#define SSID "Mensah's Nokia" // smart hub SSID
#define PASS "lucille2"       // smart hub password

//////////////////////////////////
#define STATION_SSID SSID
#define STATION_PASSWORD PASS
#define HOSTNAME "HTTP_BRIDGE"
#define AP_SSID "."
#define AP_PASS "lucille2"
/////////////////////////////////
#define RELAY_PIN D1  // relay
#define SERVER_PIN D2 // relay
#define WIFI_PIN D3
// #define SERVER_PATH "http://10.42.0.1/summary/current"
#define SERVER_PATH "http://api.freerealapi.com/comments/618a862db883dbaafa9449c5"
unsigned long previousMillis = 0;
unsigned long interval = 5000;
String deviceName = "Bless"; // Set this value for each smart socket. THeir names are under the esp82366
#define NODE_ID ASCIISentence(deviceName)
bool wifiConnected = false;
bool isRoot = true;
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
DynamicJsonDocument staticDoc(1024);
bool toggle = false; // pin status of node's relay
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
JSONVar parseJson;
AsyncWebServer server(80);
IPAddress myIP(0, 0, 0, 0);
IPAddress myAPIP(0, 0, 0, 0);