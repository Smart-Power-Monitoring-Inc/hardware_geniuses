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

void connectToWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS); // Connect to the network
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        i++;

        if (i == 100)
        {
            errorBlin_NO_CONNECTION();
            break;
        }
        Serial.print('.');
        errorBlink();
    }
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}

void createHotspot()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    myAPIP = WiFi.softAPIP();
    Serial.print("AP IP address:\t");
    Serial.println(myAPIP);
    HOTSPOT_CREATED();
}

void postData(String data)
{ // called when a node sends a message
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    if ((WiFi.status() == WL_CONNECTED))
    {

        if (http.begin(client, SERVER_PATH))
        {
            // int httpCode = http.POST(data);
            int httpCode = http.GET();
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
    else
    {
        Serial.println("WiFi is not connected");
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
    Serial.println("Connected to WiFi successfully");
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
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6);
    // mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();

    mesh.stationManual(SSID, PASS);
    mesh.setHostname(HOSTNAME);

    // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", "<h1>Hello, Welcome to Keeps's Smaart Power Monitoring Site!</h1>"); });

    server.begin();

    myAPIP = IPAddress(mesh.getAPIP());
    Serial.println("My AP IP is " + myAPIP.toString());
}

void getWiFiConnectionStatus()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        HOTSPOT_CREATED();
        switch (WiFi.status())
        WiFi.
        previousMillis = currentMillis;
    }
}

void HOTSPOT_CREATED()

{
    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(WIFI_PIN, HIGH);
    }
    else
    {
        digitalWrite(WIFI_PIN, LOW);
    }
}

String getCurrentReading()
{
    JSONVar doc;
    String json = "";
    doc["id"] = NODE_ID;
    doc["current"] = analogRead(A0);
    // doc["time"] = millis();
    doc["name"] = deviceName;
    doc["command"] = "";
    json = JSON.stringify(doc);

    // Not on node 1
    // Used to get the current readings
    // Only used every other node except node 1
    // return current readings
    return json;
}

// Not on node 1
void switchOffNodeRelay()
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