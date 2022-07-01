// Function Prototypes
void connectToWiFi();
int ASCIISentence(String str);
void errorBlin_NO_CONNECTION();
void sendMessage(); // Prototype so PlatformIO doesn't complain
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
void sendMessageToNode(u_int32_t nodeId);
String getCurrentReading();
void switchOffNodeRelay();
void postData(String data);
void errorBlink();
void HOTSPOT_CREATED();
JSONVar decodeJSON(String json);
void initMesh();
void getWiFiConnectionStatus();
void errorBlin_NO_CONNECTION(bool state);
void createHotspot();