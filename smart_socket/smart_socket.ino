#define CURRENT A0  // Analog pin for nodeMCU

void setup() {
  // put your setup code here, to run once:
  pinMode(CURRENT, INPUT);   // setting A0 as read
}

void loop() {
  // put your main code here, to run repeatedly:
  
}


// Measuring Current
float current() {
  float resolution = 3.3 / 1024; // Input Voltage Range is 1V to 3.3V
                                 // ESP8266 ADC resolution is 10-bit. 2^10 = 1024
  float rawCurrent = analogRead(CURRENT);
  float actCurrent = rawCurrent * resolution;
  Serial.print(actCurrent, 3);
  return actCurrent;
}

float voltage() {
  // Voltage measuring code here
}

void forwardData() {
 // code to forward current and voltage to smart hub
}

void connectToHub() {
  // code to connect to smart hub network
}

void switchDevice() {
  // code to turn off/on device remotely
}
