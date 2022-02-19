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


static const char AUX_mqtt_setting[] PROGMEM = R"raw(
[
  {
    "title": "MQTT Setting",
    "uri": "/mqtt_setting",
    "menu": true,
    "element": [
      {
        "name": "style",
        "type": "ACStyle",
        "value": "label+input,label+select{position:sticky;left:120px;width:230px!important;box-sizing:border-box;}"
      },
      {
        "name": "header",
        "type": "ACText",
        "value": "<h2>MQTT broker settings</h2>",
        "style": "text-align:center;color:#2f4f4f;padding:10px;"
      },
      {
        "name": "caption",
        "type": "ACText",
        "value": "Publishing the WiFi signal strength to MQTT channel. RSSI value of ESP8266 to the channel created on ThingSpeak",
        "style": "font-family:serif;color:#4682b4;"
      }
    ]
  }
]
)raw";


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
    void setupAcPages();
};


#endif
