#ifndef NETWORK_APP_H
#define NETWORK_APP_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AutoConnect.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>

#include "../mymqttwrapper.h"


class NetworkApp {
  private:
    WebServer server;
    AutoConnect portal;
    AutoConnectConfig config;

    WiFiClient espClient;
    PubSubClient mqttClient;

    WiFiUDP ntpUDP;
    NTPClient timeClient;

    MyMqttWrapper myMqttWrapper;

  public:
    NetworkApp() 
        : portal(server)
        , mqttClient(espClient)
        , timeClient(ntpUDP)
        , myMqttWrapper(&mqttClient, &timeClient)
        {}
    //NetworkApp(PubSubClient * mqttClient, NTPClient * timeClient);

    void setup();
    bool loop();


};


#endif
