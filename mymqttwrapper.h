#ifndef MY_MQTT_WRAPPER_H
#define MY_MQTT_WRAPPER_H

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <NTPClient.h>

class MyMqttWrapper {
  private:
    int index;
    PubSubClient * mqttClient;
    NTPClient * timeClient;
    String topic;
    bool disabled;
    
  public:
    MyMqttWrapper();
    MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient);
    void publish(JsonDocument& document);
    void setTopic(String topic);
};

#endif
