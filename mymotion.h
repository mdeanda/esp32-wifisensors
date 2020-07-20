#ifndef MY_MOTION_H
#define MY_MOTION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

#include "mymqttwrapper.h"
#include "mylcdprovider.h"

class MyMotion {
  private:
    int pin;
    volatile bool hasMotion;
    MyMqttWrapper * mqtt;
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


    
  public:
    MyMotion(const int pin, MyMqttWrapper * myMqttWrapper);

    void start();
    void loop();
    
};

#endif
