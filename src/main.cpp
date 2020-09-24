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

MyButton myButton1(BUTTON_01, 50, "01");
MyButton myButton2(BUTTON_02, 50, "02");
//MyButton myButton3(BUTTON_03, 50);
//MyButton myButton4(BUTTON_04, 50);
MyButton myButton5(BUTTON_05, 50, "05");
MyButton myButton6(BUTTON_06, 50, "06");
MyButton myButton7(BUTTON_07, 50, "07");

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
  pinMode(LED_PIN, OUTPUT);
  pinMode(LUMIN_PIN, INPUT);

  for (int i=0; i<BUTTON_COUNT; i++) {
    buttons[i].setup();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  /*
  int v = analogRead(LUMIN_PIN) / 4 + 25;
  delay(v);
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  // */

  //*
  for (int i=0; i<BUTTON_COUNT; i++) {
    if (buttons[i].loop()) {
      Serial.println("button pressed: " + buttons[i].getLabel());
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }
  delay(10);
  //*/
}
