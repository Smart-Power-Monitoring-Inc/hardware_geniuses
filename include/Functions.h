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

JSONVar decodeJson(String json)
{
    parseJson = JSON.parse(json.c_str());
    int nodeId = parseJson["id"];               // short node id
    const char *command = parseJson["command"]; // command to be issued to the node
    const char *current = parseJson["current"]; // node current
    const char *name = parseJson["name"];       // node name
    return parseJson;
}

void dataTransferBlink()
{
    digitalWrite(AMBER, HIGH);
    delay(100);
    digitalWrite(AMBER, LOW);
    delay(100);
    digitalWrite(AMBER, HIGH);
    delay(100);
    digitalWrite(AMBER, LOW);
    delay(10);
}

void dataReceivedBlink()
{
    digitalWrite(BLUE, HIGH);
    delay(100);
    digitalWrite(BLUE, LOW);
    delay(50);
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> New Connection, nodeId = %u\n", nodeId);
    dataReceivedBlink();
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections\n");
    dataReceivedBlink();
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
    const char *uid = parseJson["uid"];
    switchOffNodeRelay(uid);
    if (isRoot)
    {
        getVoltageReading();
    }
    dataReceivedBlink();
}

void initMesh()
{
    wifiConnectHandler = WiFi.onStationModeGotIP(onWiFiConnect);
    wifiDisconnectHandler = WiFi.onSoftAPModeStationDisconnected(onWiFiDisconnect);
    // connectToWiFi(); // Comment this out if uploading to other nodes. Only use this for node 1
    mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE); // all types on
    // mesh.setDebugMsgTypes(ERROR | STARTUP);                                                                        // set before init() so that you can see startup messages
    mesh.init(AP_SSID, AP_PASS, &userScheduler, MESH_PORT, WIFI_AP_STA, 6);
    // mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();

    if (isRoot)
    {
        mesh.setRoot(true);
        mesh.setContainsRoot(true);
        server.on("/", homeRoute);
        server.on("/user", HTTP_POST, updateAccountID);
        server.on("/toogle", HTTP_POST, broadcast);
        server.begin();
        myAPIP = IPAddress(mesh.getAPIP());
        Serial.println("My AP IP is " + myAPIP.toString());
    }
    else
    {
        // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
        mesh.setContainsRoot(true);
    }
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

void broadcast()
{
    String json = "";
    if (server.method() == HTTP_POST)
    {
        Serial.println(server.arg(0));
        String data = server.arg(0);
        json += data;
        mesh.sendBroadcast(json);
        Serial.print("Relay Toggle Command: ");
        Serial.println(json);
    }
    server.send(200, "text/plain", "Done");
}

String getVoltageReading()
{
    WiFiClient client;
    HTTPClient http;
    JSONVar doc;
    String json = "";
    doc["id"] = NODE_ID;
    json = JSON.stringify(doc);
    // int R1 = 30000; // before intersection
    // int R2 = 7500;  // ground
    // float measuredVoltage = ((R2 / (R2 + R1)) * 9);
    int avg = 0;
    // Get average v reading
    for (int i = 0; i < 100; i++)
    {
        avg += analogRead(A0);
    }
    avg /= 100;
    float float_avg = String(avg).toFloat();
    float first_cnv = (float_avg * 5 / 1023);
    // 1.8v == 220 -240
    // new reading is ?
    // 5 =240
    float val = (first_cnv / 5) * 220;
    doc["voltage"] = val;
    String path = VOLTAGE_PATH + "?value=" + val;
    Serial.print("Measured voltage: ");
    Serial.println(val);
    if (http.begin(client, path))
    {
        // int httpCode = http.POST(data);
        int httpCode = http.POST(json);
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            String payload = http.getString();
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = http.getString();
                // Serial.println(payload);
                // Decode JSON
                // decodeJSON(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
    }
    return json;
}

void switchOffNodeRelay(String id)
{
    Serial.print("This is the node id: ");
    Serial.println(id);
    if (id == String(NODE_ID))
    {
        digitalWrite(RELAY_PIN, HIGH);
        delay(3000);
        digitalWrite(RELAY_PIN, LOW);
        if (!isRoot) // Run only on other nodes
        {
            if (toggle)
            {
                digitalWrite(RELAY_PIN, LOW);
                toggle = !toggle;
            }
            else
            {
                digitalWrite(RELAY_PIN, HIGH);
                toggle = !toggle;
            }
        }
    }
}

void sendMessage()
{
    String msg = getCurrentReading();
    mesh.sendBroadcast(msg);
    taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
    dataTransferBlink();
    digitalWrite(RELAY_PIN, LOW);
}

IPAddress getlocalIP()
{
    return IPAddress(mesh.getStationIP());
}