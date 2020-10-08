#ifndef NETWORK_APP_H
#define NETWORK_APP_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AutoConnect.h>
#include <ArduinoJson.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>

#include "mymqttwrapper.h"


class NetworkApp : MqttListener {
  private:
    WebServer server;
    AutoConnect portal;
    AutoConnectConfig config;

    WiFiClient espClient;
    PubSubClient mqttClient;

    WiFiUDP ntpUDP;
    NTPClient timeClient;

    MyMqttWrapper myMqttWrapper;

    unsigned long disconnectTime = 0;
    const unsigned long rebootTime = 300000;

    unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectAttemptInterval = 15000;

    const unsigned long onlineMessageInterval = 300000;
    unsigned long nextOnlineMessage = 0;

    bool justReconnected = false;

  public:
    NetworkApp() 
        : portal(server)
        , mqttClient(espClient)
        , timeClient(ntpUDP)
        , myMqttWrapper(&mqttClient, &timeClient)
        {}

    void setup();
    bool loop();
    bool isReconnected();

    void updateStatus(const char * statusType, JsonDocument &value);
    virtual void onMessage(char* topic, char* payload, unsigned int length) {
      Serial.print(topic);
      Serial.print(" ");
      Serial.println(String(payload));
    };

  private:
    void trackDisconnect(int code);
    void rebootWatchdog();
    void sendOnlineMessage();

};


#endif
