#include <Arduino.h>

/*
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AutoConnect.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
//*/

#include <mylcd.h>
#include <mymqttwrapper.h>
#include <mymotion.h>
#include <myluminance.h>
#include <mytemperature.h>
#include <mybutton.h>
#include <mytimedswitch.h>
//#include <mythrottle.h>
//#include <mycontactswitch.h>

#include <garagedoorapp.h>

#define LED_PIN 2
#define LUMIN_PIN 35

#define BUTTON_01 27 // ok
#define BUTTON_02 26 // ok
//#define BUTTON_03 25 // touch?
//#define BUTTON_04 33 // touch?
#define BUTTON_05 14 // ok
#define BUTTON_06 12 // ok
#define BUTTON_07 13 // ok
const int BUTTON_COUNT = 5;

MyButton myButton1(BUTTON_01, 20, "01");
MyButton myButton2(BUTTON_02, 20, "02");
//MyButton myButton3(BUTTON_03, 50);
//MyButton myButton4(BUTTON_04, 50);
MyButton myButton5(BUTTON_05, 20, "05");
MyButton myButton6(BUTTON_06, 20, "06");
MyButton myButton7(BUTTON_07, 20, "07");

GarageDoorApp myApp;
MyTimedSwitch led(LED_PIN, 5000);

MyButton buttons[BUTTON_COUNT] = {
  myButton1, 
  myButton2,
  //myButton3,
  //myButton4,
  myButton5,
  myButton6,
  myButton7
};

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Hello world");

  // put your setup code here, to run once:
  led.setup();
  pinMode(LUMIN_PIN, INPUT);

  for (int i=0; i<BUTTON_COUNT; i++) {
    buttons[i].setup();
  }

  myApp.setup();
  Serial.println("setup done");
}

void loop() {
  myApp.loop();
  delay(10);
}
