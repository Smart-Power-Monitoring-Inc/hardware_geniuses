#include "Arduino.h"
#include "Libraries.h"
#include "Variables.h"
#include "Prototypes.h"

int ASCIISentence(String str)
{
    int l = str.length();
    int convert = 0;
    for (int i = 0; i < l; i++)
    {
        convert = str[i] - NULL;
    }
    return convert;
}

void connectToWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password); // Connect to the network
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        i++;
        WIFI_CONNECTED(false);
        if (i == 100)
        {
            errorBlin_NO_CONNECTION();
            break;
        }
        Serial.print('.');
        errorBlink();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WIFI_CONNECTED(true);
    }

    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}

void postData(String data)
{ // called when a node sends a message
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    if ((WiFi.status() == WL_CONNECTED))
    {
        if (http.begin(SERVER_PATH, client))
        {
            int httpCode = http.POST(data);
            if (httpCode > 0)
            {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET... code: %d\n", httpCode);
                String payload = http.getString();
                if (httpCode == HTTP_CODE_OK)
                {
                    String payload = http.getString();
                    Serial.println(payload);
                    // Decode JSON
                    decodeJson(payload);
                }
            }
            else
            {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
        }
        else
        {
            Serial.printf("[HTTP} Unable to connect\n");
        }
    }
}

JSONVar decodeJson(String json)
{
    parseJson = JSON.parse(json.c_str());
    int nodeId = parseJson["id"];               // short node id
    const char *command = parseJson["command"]; // command to be issued to the node
    const char *current = parseJson["current"]; // node current
    const char *name = parseJson["name"];       // node name
    return parseJson;
}

void errorBlink()
{
    // do something here
    digitalWrite(SERVER_PIN, HIGH);
    delay(500);
    digitalWrite(SERVER_PIN, LOW);
    delay(200);
}
void errorBlin_NO_CONNECTION(bool state)
{
    // do something here
    while (state)
    {
        digitalWrite(SERVER_PIN, HIGH);
        delay(500);
        digitalWrite(SERVER_PIN, LOW);
        delay(200);
    }
}

void errorBlin_NO_CONNECTION()
{
    while (true)
    {
        digitalWrite(SERVER_PIN, HIGH);
        delay(200);
        digitalWrite(SERVER_PIN, LOW);
        delay(200);
    }
}

void newConnectionCallback(uint32_t nodeId)
{
    // %u short unsigned interget
    //  %s String
    Serial.printf("--> New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void onWiFiConnect(const WiFiEventStationModeGotIP &event)
{
    Serial.print("Connected to WiFi successfully");
}

void onWiFiDisconnect(const WiFiEventSoftAPModeStationDisconnected &event)
{
    Serial.println("trying to connect");
}

void receivedCallback(uint32_t from, String &msg)
{
    // check if the incoming message contains a nodeid
    // if the id matches the id of the node, parse the data
    // %u short unsigned interget
    //  %s String
    Serial.printf("Received from %u msg=%s\n ", from, msg.c_str());
    // parse data here
    // JSONVar parseJson = JSON.parse(msg.c_str());
    // int nodeId = parseJson["id"];               // short node id
    // const char *command = parseJson["command"]; // command to be issued to the node
    // const char *current = parseJson["current"]; // node current
    // const char *name = parseJson["name"];       // node name
    //                                             /*
    //                                             If command == switch
    //                                             toggle on/off the node's relay
    //                                             if command  == node
    //                                             error = node should parse the error command
    //                                             */
    // if (command == "switch" && nodeId == NODE_ID)
    // {
    //     // ensure the correct node toggles switch
    //     switchOffNodeRelay();
    // }
    // if (command == "error")
    // {
    //     // call error functions
    //     errorBlink();
    // }
    // // only for node 1
    // postData("sdfsdfsdf");
}

void initMesh()
{
    wifiConnectHandler = WiFi.onStationModeGotIP(onWiFiConnect);
    wifiDisconnectHandler = WiFi.onSoftAPModeStationDisconnected(onWiFiDisconnect);
    // connectToWiFi(); // Comment this out if uploading to other nodes. Only use this for node 1

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

void getWiFiConnectionStatud()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        switch (WiFi.status())
        {
        case WL_CONNECTED:
            Serial.println("WiFi connected");
            Serial.println(WiFi.softAPIP());
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Connection failed");
            break;

        case WL_NO_SSID_AVAIL:
            Serial.println("SSID not available");
            break;
        case WL_IDLE_STATUS:
            Serial.println("Idle status");
            // connectToWiFi();
            break;
        default:
            break;
        }
        previousMillis = currentMillis;
        Serial.print("WIFI STATUS ");
        Serial.println(WiFi.status());
    }
}

void WIFI_CONNECTED(bool state)
{
    if (state)
    {
        digitalWrite(WIFI_PIN, HIGH);
    }
    else
    {
        digitalWrite(WIFI_PIN, LOW);
    }
}