
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
#define DEVICE_NAME isRoot ? "ROOT" : "Bless"
station_info stationInfo;
int count;
/////////////////////////////////
const int RELAY_PIN = D5; // relay
#define AMBER 12
#define RED D7
#define BLUE D8
#define VOLTAGE D2
#define CURRENT D3

// #define SERVER_PATH "http://10.88.138.101:3000/summary/current?accountId="
#define SERVER_PATH "http://10.192.18.110:3000/summary/current?accountId="
#define VOLTAGE_PATH "http://10.192.18.110:3000/summary/" + accountId + "/voltage"
// #define SERVER_PATH "http://api.freerealapi.com/comments/618a862db883dbaafa9449c5"
unsigned long previousMillis = 0;
unsigned long interval = 5000;
#define NODE_ID ASCIISentence(DEVICE_NAME)
bool wifiConnected = false;
bool isRoot = true;
// String deviceName = isRoot ? "ROOT" : "Bless"; // Set this value for each smart socket. THeir names are under the esp82366

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
DynamicJsonDocument staticDoc(1024);
bool toggle = false; // pin status of node's relay
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
JSONVar parseJson;
ESP8266WebServer server(80);
IPAddress myIP(0, 0, 0, 0);
IPAddress myAPIP(0, 0, 0, 0);
String accountId = "62cc2c675065faa3815663ec";
String uid = "";