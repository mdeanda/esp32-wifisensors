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
    MY_MQTT_CALLBACK_SIGNATURE;
    String clientName;
    
  public:
    MyMqttWrapper();
    MyMqttWrapper(PubSubClient * mqttClient, NTPClient * timeClient, MY_MQTT_CALLBACK_SIGNATURE);
    void setClientName(String clientName);
    void publish(JsonDocument& document);
    void setTopic(String topic);
    bool loop();

  private:
    void sayHello();
    bool reconnectMqtt();
};

#endif
