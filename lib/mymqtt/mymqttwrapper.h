#ifndef MY_MQTT_WRAPPER_H
#define MY_MQTT_WRAPPER_H

#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <NTPClient.h>

#include "mqttlistener.h"

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define MY_MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MY_MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)
#endif


class MyMqttWrapper {
  private:
    int index;
    PubSubClient * mqttClient;
    NTPClient * timeClient;
    String topic;
    bool disabled;
    //String clientId;
    String clientName;
    String inTopic;
    MqttListener * listener;

    static std::vector<MyMqttWrapper *> listeners;
    
  public:
    MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient);
    void setup();
    void setClientName(String clientName);
    void publish(JsonDocument& document);
    void publish(const char *topic, JsonDocument& document);
    void setTopic(String topic);
    void setInTopic(String inTopic);
    void setListener(MqttListener * listener);
    bool loop();
    
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    void messageReceived(char* topic, byte* payload, unsigned int length);

  private:
    bool reconnectMqtt();
};

#endif
