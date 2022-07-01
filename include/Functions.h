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
        convert = str[i];
    }
    return convert;
}

void postData(String data)
{ // called when a node sends a message
    WiFiClient client;
    HTTPClient http;
    String path = SERVER_PATH;
    path += accountId;
    Serial.println(path);
    if (accountId == "") // Only proceed when the account id is set
        return;
    Serial.print("[HTTP] begin...\n");
    
    if (http.begin(client, path))
    {
        // int httpCode = http.POST(data);
        Serial.println("Data Posted: " + data);
        http.addHeader("Content-type", "application/json");
        int httpCode = http.POST(data);
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            String payload = http.getString();
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                Serial.println(payload);
                // Decode JSON
                // decodeJSON(payload);
                errorBlin_NO_CONNECTION(false);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            errorBlin_NO_CONNECTION(true);
        }
    }
    else
    {
        Serial.printf("[HTTP} Unable to connect\n");
        errorBlin_NO_CONNECTION(true);
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
    digitalWrite(SERVER_PIN, HIGH);
    delay(100);
    digitalWrite(SERVER_PIN, LOW);
    delay(300);
    digitalWrite(SERVER_PIN, HIGH);
    delay(500);
    digitalWrite(SERVER_PIN, LOW);
    delay(700);
}

void errorBlin_NO_CONNECTION()
{

    digitalWrite(SERVER_PIN, HIGH);
    delay(200);
    digitalWrite(SERVER_PIN, LOW);
    delay(200);
}

void newConnectionCallback(uint32_t nodeId)
{
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
    Serial.println("Connected to WiFi successfully");
}

void onWiFiDisconnect(const WiFiEventSoftAPModeStationDisconnected &event)
{
    Serial.println("trying to connect");
}

void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("Received from %u msg=%s\n ", from, msg.c_str());
    // parse data here
    JSONVar parseJson = JSON.parse(msg.c_str());
    postData(JSON.stringify(parseJson));
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
    mesh.init(AP_SSID, AP_PASS, &userScheduler, MESH_PORT, WIFI_AP_STA, 6);
    // mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();
    HOTSPOT_CREATED();

    if (isRoot)
    {
        mesh.setRoot(true);
    }
    else
    {
        // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
        mesh.setContainsRoot(true);
    }
    // Async webserver
    server.on("/", homeRoute);
    server.on("/user", HTTP_POST, updateAccountID);
    server.begin();
    myAPIP = IPAddress(mesh.getAPIP());
    Serial.println("My AP IP is " + myAPIP.toString());
}

void homeRoute()
{
    server.send(200, "text/plain", "<h1>Hello, Welcome to Keeps's Smaart Power Monitoring Site!</h1>");
}

void updateAccountID()
{
    if (server.method() == HTTP_POST)
    {

        if (server.hasArg("id"))
        {
            accountId = server.arg("id");
            Serial.println("Account Id: " + accountId);
        }
    }
    server.send(200, "text/plain", "Done");
}

void HOTSPOT_CREATED()

{
    digitalWrite(WIFI_PIN, HIGH);
}

String getCurrentReading()
{
    JSONVar doc;
    String json = "";
    doc["id"] = NODE_ID;
    doc["current"] = analogRead(A0);
    doc["name"] = DEVICE_NAME;
    doc["command"] = "";
    json = JSON.stringify(doc);
    return json;
}

void switchOffNodeRelay()
{
    // Not on node 1
    if (!isRoot) // Run only on other nodes
    {
        if (toggle)
        {
            digitalWrite(RELAY_PIN, HIGH);
            toggle = !toggle;
        }
        else
        {
            digitalWrite(RELAY_PIN, LOW);
            toggle = !toggle;
        }
    }
}

void sendMessage()
{
    String msg = getCurrentReading();
    mesh.sendBroadcast(msg);
    taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

IPAddress getlocalIP()
{
    return IPAddress(mesh.getStationIP());
}