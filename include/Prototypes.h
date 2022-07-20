// Function Prototypes
void connectToWiFi();
int ASCIISentence(String str);
void sendMessage(); // Prototype so PlatformIO doesn't complain
void sendMessageToNode(u_int32_t nodeId);
void switchOffNodeRelay(String id);
void postData(String data);
void initMesh();
void getWiFiConnectionStatus();
void homeRoute();
void updateAccountID();
void dataTransferBlink();
void dataReceivedBlink();
String getVoltageReading();
JSONVar decodeJSON(String json);
String getCurrentReading();
void broadcast();
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
