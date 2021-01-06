#include <Arduino.h>

#include <garagedoorapp.h>
#include <doublegateapp.h>

//GarageDoorApp myApp;
DoubleGateApp myApp;


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Hello world");

  myApp.setup();
  Serial.println("setup done");
}

void loop() {
  myApp.loop();
  delay(10);
}
