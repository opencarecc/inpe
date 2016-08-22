/*
  Rephone Kit Hello World
  
  Hello, this is a very simple sketch to test the
  communication between your computer and the
  Rephone's main module, Xadow GSM+BLE.
  
  INSTRUCTIONS:
  Select "Debug Port" from Tools > Port menu.
  After successful upload, change the port to "Modem Port".
  Open Serial Monitor and set baud rate at 115200.
  
  HARDWARE:
  - Xadow GSM+BLE
  
  Alessandro Contini for http://opencare.cc/
  August 2016
*/

void setup() {
  // start serial communication
  Serial.begin(115200);
}

void loop() {
  // print Hello World! on the Serial Monitor
  Serial.println("Hello World!");
  delay(100);
}
