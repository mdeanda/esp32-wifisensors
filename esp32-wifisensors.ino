/*
 * runs sensors and sends data to mqtt server
 * sensors: DHT22 (3v3), Motion (5v)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AutoConnect.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>

#include "src/mylcd.h"
#include "src/timeprovider.h"
#include "src/mymqttwrapper.h"
#include "src/mytemperature.h"
#include "src/myluminance.h"
#include "src/mydebounce.h"

#include "src/app/networkapp.h"

/**
 * ----------------------------------------
 * include "config.h"
 * ----------------------------------------
 * MQTT_SERVER    -- mqtt server name
 *
 * LED_PIN 2
 * LUMIN_PIN 27
 * MOTION_PIN 26
 * DHTPIN 4
 * 
 * LUMINANCE_INTERVAL_MS 5000
 * MOTION_INTERVAL_MS 300
 * DHT_INTERVAL_SEC 60
 * //analog range: 0 to 4095,
 * ----------------------------------------
 */
#include "config.h"


#ifndef ESP32
#pragma message(THIS IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

#define BUTTON_01 10 //--panic
#define BUTTON_02 9  //--no interrupt
#define BUTTON_03 8  //--no
#define BUTTON_04 7  //--no
#define BUTTON_05 11 //--panic
#define BUTTON_06 12 // ok
#define BUTTON_07 13 // ok
#define DEBOUNCE_THRESHOLD 150

WebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

MyMqttWrapper myMqttWrapper(&mqttClient, &timeClient);
MyTemperature myTemperature(DHTPIN, DHT_INTERVAL_SEC, &myMqttWrapper);
MyLuminance myLuminance(LUMEN_PIN, LUMINANCE_INTERVAL_MS, LUMEN_THRESHOLD, &myMqttWrapper);
sensors::TimeProvider timeProvider(&timeClient);

int lcdColumns = 20;
int lcdRows = 4;
int lcdRow = 0;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
MyLcd myLcd(&lcd, lcdRows, lcdColumns);

MyDebounce gateContact6(BUTTON_06, DEBOUNCE_THRESHOLD);
MyDebounce gateContact7(BUTTON_07, DEBOUNCE_THRESHOLD);

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  gateContact6.setup();
  gateContact7.setup();

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.clear();

  mqttClient.setServer(MQTT_SERVER, 1883);

  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.println("Hello world");

  config.autoReconnect = true;
  config.principle = AC_PRINCIPLE_RSSI;
  //config.apid = "ESP-" + String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  portal.config(config);
  if (portal.begin()) {
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
    String localIp = WiFi.localIP().toString();
    localIp.replace(".", "_");
  
    Serial.println(localIp);
    myMqttWrapper.setTopic(localIp);
    myMqttWrapper.setClientName(localIp);

    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  myMqttWrapper.setup();


  timeClient.begin();
  
  delay(500);

  myTemperature.start();
  myLuminance.start();

  myLcd.addSsMessageProvider(&timeProvider);
  myLcd.addSsMessage("Hello world");
  myLcd.addSsMessage("it works!");
  /*
  myLcd.addSsMessage("yes it does");
  myLcd.addSsMessage("...");
  myLcd.addSsMessage("5 lines?");
  myLcd.addSsMessage("even 6!");
  //*/
  
  myLcd.addSsMessageProvider(&myTemperature);

}


void loop()
{
  server.handleClient();
  portal.handleClient();

  timeClient.update();


  //TODO: we're getting past this before connection is fully established (clientId is blank)
  while (WiFi.status() == WL_IDLE_STATUS) {
    Serial.println("Not connected to WiFi");
    // do we really want this?
    delay(500);
    ESP.restart();
    delay(500);
    return;
  }
  
  if (!myMqttWrapper.loop()) {
    delay(5000);
    return;
  }

  myLcd.loop();


  if (gateContact6.loop()) {
    Serial.println("gateContact6 pressed");
  }

  if (gateContact7.loop()) {
    Serial.println("gateContact7 pressed");
  }

  delay(100);

}
