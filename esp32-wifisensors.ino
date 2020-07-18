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

#include "mylcd.h"
#include "timeprovider.h"
#include "mymqttwrapper.h"
#include "mytemperature.h"
#include "myluminance.h"

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

WebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

String clientName; //defined by ip address;
int messageId = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int lastMotionValue = HIGH;
TaskHandle_t motionTaskHandle = NULL;
Ticker motionTicker;

MyMqttWrapper myMqttWrapper(&mqttClient, &timeClient);
MyTemperature myTemperature(DHTPIN, DHT_INTERVAL_SEC, &myMqttWrapper);
MyLuminance myLuminance(LUMEN_PIN, LUMINANCE_INTERVAL_MS, LUMEN_THRESHOLD, &myMqttWrapper);
sensors::TimeProvider timeProvider(&timeClient);

int lcdColumns = 20;
int lcdRows = 4;
int lcdRow = 0;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
MyLcd myLcd(&lcd, lcdRows, lcdColumns);


portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool motionNow = false;

boolean initMotion() {

  xTaskCreate(
      motionTask,                     // Function to implement the task
      "motionTask ",                  // Name of the task
      4000,                           // Stack size in words
      NULL,                           // Task input parameter
      5,                              // Priority of the task
      &motionTaskHandle);             // Task handle.

  if (motionTaskHandle == NULL) {
    Serial.println("Failed to start task for motion update");
    return false;
  } else {
    // Start update of environment data every DHT_INTERVAL_SEC seconds
    motionTicker.attach_ms(MOTION_INTERVAL_MS, triggerGetMotion);
  }
  return true;
}

void detectsMovement() {
  String msg = "\"motion\": 1";
  publishMqtt(msg.c_str());
}

void triggerGetMotion() {
  if (motionTaskHandle != NULL) {
     xTaskResumeFromISR(motionTaskHandle);
  }
}

void motionTask(void *pvParameters) {
  Serial.println("motionTask loop started");
  while (1)
  {
    getMotion();
    // Got sleep again
    vTaskSuspend(NULL);
  }
}

bool getMotion()
{
  //Serial.println("motion check");
  int motionValue = digitalRead(MOTION_PIN);
  if (motionValue != lastMotionValue) {
    String msg = "\"motion\": \"" + String(motionValue) + "\"";
    publishMqtt(msg.c_str());
  }
  lastMotionValue = motionValue;
  return true;
}


void IRAM_ATTR motionInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  motionNow = true;
  portEXIT_CRITICAL_ISR(&mux);
}


void publishMqtt(const char* msg)
{
  if (!mqttClient.connected()) {
    Serial.print("not connected: ");
    Serial.println(msg);
    return;
  } else {
    Serial.println(msg);
  }

  String topic = clientName;
  messageId++;
  String id = String(messageId);
  String string_msg = "{\"id\": " + id + ", " + String(msg) + "}";
  mqttClient.publish(topic.c_str(), string_msg.c_str());
}

void sayHello()
{
  String msg = "\"message\": \"hello world\"";
  publishMqtt(msg.c_str());
}

bool reconnectMqtt()
{
  // Loop until we're reconnected
  Serial.print("mqtt...");
  if (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(clientName.c_str());

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sayHello();
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttClient.state());
      return false;
    }
  }

  return mqttClient.connected();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}



void setup()
{
  pinMode(LED_PIN, OUTPUT);
  //pinMode(MOTION_PIN, INPUT);
  pinMode(MOTION_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), motionInterrupt, RISING);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.clear();

  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(mqttCallback);

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
    clientName = localIp;
    myMqttWrapper.setTopic(clientName);

    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  timeClient.begin();
  
  delay(500);

//  initMotion();
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
  
  if (!mqttClient.connected()) {
    if (!reconnectMqtt()) {
      // Wait 5 seconds before retrying
      delay(5000);
      return;
    }
  }
  mqttClient.loop();
  myLcd.loop();

  if (motionNow) {
    String msg = "\"motion\": \"interrupt\"";
    publishMqtt(msg.c_str());
    portENTER_CRITICAL(&mux);
    motionNow = false;
    portEXIT_CRITICAL(&mux);
  }


  delay(100);



}
