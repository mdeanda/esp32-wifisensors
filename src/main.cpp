#include <Arduino.h>

#include <garagedoorapp.h>
#include <doublegateapp.h>
#include <sensorapp.h>
#include <pooltempsensorapp.h>

//GarageDoorApp myApp;
//DoubleGateApp myApp;
//SensorApp myApp;
PoolTempSensorApp myApp;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();

  myApp.setup();
}

void loop() {
  myApp.loop();
  delay(10);
}
