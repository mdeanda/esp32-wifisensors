/*
 * runs sensors and sends data to mqtt server
 * sensors: DHT22 (3v3), Motion (5v)
 */

#include <WiFi.h>
#include "DHTesp.h"
#include "Ticker.h"
#include <PubSubClient.h>

/**
 * ----------------------------------------
 * include "config.h"
 * ----------------------------------------
 * SSID           -- wifi ssid
 * PASSWORD       -- wifi password
 * MQTT_SERVER    -- mqtt server name
 * MQTT_OUT_TOPIC -- name of topic
 * USE_WIFI       -- just empty 
 *    #define USE_WIFI to enable wifi code
 * ----------------------------------------
 */
#include "config.h"


#define LED_PIN 2
#define MOTION_PIN 26
#define MOTION_INTERVAL_MS 300
#define DHTPIN 4
#define DHT_INTERVAL 60


#ifndef ESP32
#pragma message(THIS IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif


String clientName; //defined by ip address;
DHTesp dht;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
int lastMotionValue = HIGH;


TaskHandle_t motionTaskHandle = NULL;
Ticker motionTicker;

void tempTask(void *pvParameters);
bool getTemperature();
void triggerGetTemp();

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Ticker for temperature reading */
Ticker tempTicker;
/** Comfort profile */
ComfortState cf;
/** Flag if task should run */
bool tasksEnabled = false;

boolean initMotion() {
  pinMode(MOTION_PIN, INPUT);

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
    // Start update of environment data every DHT_INTERVAL seconds
    motionTicker.attach_ms(MOTION_INTERVAL_MS, triggerGetMotion);
  }
  return true;
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
    if (tasksEnabled) {
      getMotion();
    }
    // Got sleep again
    vTaskSuspend(NULL);
  }
}

bool getMotion()
{
  if (!tasksEnabled) return false;

  //Serial.println("motion check");
  int motionValue = digitalRead(MOTION_PIN);
  //if (motionValue == HIGH) {
  if (motionValue != lastMotionValue) {
    //Serial.println("motion!");
    String msg = "{\"clientId\": \"" + clientName + "\", \"motion\": \"" + motionValue + "\"}";
    publishMqtt(msg.c_str());
  } else {
    //Serial.println("no motion");
  }
  lastMotionValue = motionValue;
  //delay(100);
  return true;
}

/**
 * initTemp
 * Setup DHT library
 * Setup task and timer for repeated measurement
 * @return bool
 *    true if task and timer are started
 *    false if task or timer couldn't be started
 */
bool initTemp() {
  // Initialize temperature sensor
  dht.setup(DHTPIN, DHTesp::DHT22);
  Serial.println("DHT initiated");

  // Start task to get temperature
  xTaskCreate(
      tempTask,                       // Function to implement the task
      "tempTask ",                    // Name of the task
      4000,                           // Stack size in words
      NULL,                           // Task input parameter
      5,                              // Priority of the task
      &tempTaskHandle);               // Task handle.

  if (tempTaskHandle == NULL) {
    Serial.println("Failed to start task for temperature update");
    return false;
  } else {
    // Start update of environment data every DHT_INTERVAL seconds
    tempTicker.attach(DHT_INTERVAL, triggerGetTemp);
  }
  return true;
}

/**
 * triggerGetTemp
 * Sets flag dhtUpdated to true for handling in loop()
 * called by Ticker getTempTimer
 */
void triggerGetTemp() {
  if (tempTaskHandle != NULL) {
     xTaskResumeFromISR(tempTaskHandle);
  }
}

/**
 * Task to reads temperature from DHT sensor
 * @param pvParameters
 *    pointer to task parameters
 */
void tempTask(void *pvParameters) {
  Serial.println("tempTask loop started");
  while (1) // tempTask loop
  {
    if (tasksEnabled) {
      // Get temperature values
      getTemperature();
    }
    // Got sleep again
    vTaskSuspend(NULL);
  }
}

/**
 * getTemperature
 * Reads temperature from DHT11 sensor
 * @return bool
 *    true if temperature could be aquired
 *    false if aquisition failed
*/
bool getTemperature() {
  if (!tasksEnabled) return false;
  
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT error status: " + String(dht.getStatusString()));
    return false;
  }

  float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
  float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
  float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

  String comfortStatus;
  switch(cf) {
    case Comfort_OK:
      comfortStatus = "OK";
      break;
    case Comfort_TooHot:
      comfortStatus = "TooHot";
      break;
    case Comfort_TooCold:
      comfortStatus = "TooCold";
      break;
    case Comfort_TooDry:
      comfortStatus = "TooDry";
      break;
    case Comfort_TooHumid:
      comfortStatus = "TooHumid";
      break;
    case Comfort_HotAndHumid:
      comfortStatus = "HotAndHumid";
      break;
    case Comfort_HotAndDry:
      comfortStatus = "HotAndDry";
      break;
    case Comfort_ColdAndHumid:
      comfortStatus = "ColdAndHumid";
      break;
    case Comfort_ColdAndDry:
      comfortStatus = "ColdAndDry";
      break;
    default:
      comfortStatus = "Unknown:";
      break;
  };

  //NOTE: message was too big and mqtt client wasn't sending it
  String msg = "{\"clientId\": \"" + clientName
      + "\", \"T\": " + String(newValues.temperature * 1.8 + 32) 
      + ", \"H\": " + String(newValues.humidity) 
      + ", \"I\": " + String(heatIndex * 1.8 + 32) 
      + ", \"D\": " + String(dewPoint * 1.8 + 32) 
      + ", \"C\": \"" + comfortStatus
      + "\"}";
  publishMqtt(msg.c_str());
  return true;
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

  mqttClient.publish(MQTT_OUT_TOPIC, msg);
}

void sayHello()
{
  String msg = "{\"clientId\": \"" + clientName + "\", \"message\": \"hello world\"}";
  publishMqtt(msg.c_str());
}

void reconnectMqtt()
{
  // Loop until we're reconnected
  Serial.print("mqtt...");
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print(clientName.c_str());

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sayHello();
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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

  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(mqttCallback);

  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

#ifdef USE_WIFI
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  String localIp = WiFi.localIP().toString();
  localIp.replace(".", "_");

  Serial.println(localIp);
  clientName = localIp;
#endif

  delay(500);
  initMotion();
  initTemp();

  tasksEnabled = true;
}


void loop()
{
  if (!tasksEnabled) {
    delay(2000);
    return;
  }

#ifdef USE_WIFI
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected to WiFi");
    delay(5000);
    return;
  }

  if (!mqttClient.connected()) {
    reconnectMqtt();
  }
  mqttClient.loop();
#endif

  yield();

}
