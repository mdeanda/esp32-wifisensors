#ifndef MY_MQTT_WRAPPER_H
#define MY_MQTT_WRAPPER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <NTPClient.h>

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
    String clientName;
    String inTopic;

    static std::vector<MyMqttWrapper *> listeners;
    
  public:
    MyMqttWrapper();
    MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient);
    void setup();
    void setClientName(String clientName);
    void publish(JsonDocument& document);
    void setTopic(String topic);
    void setInTopic(String inTopic);
    bool loop();
    
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    void messageReceived(char* topic, byte* payload, unsigned int length);

  private:
    void sayHello();
    bool reconnectMqtt();
};

#endif
