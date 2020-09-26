#ifndef MQTT_LISTENER_H
#define MQTT_LISTENER_H

#include <Arduino.h>

class MqttListener {
    public:
    virtual void onMessage(char* topic, char* payload, unsigned int length) = 0;
};

#endif