#ifndef MY_LUMINANCE_H
#define MY_LUMINANCE_H

#include <Ticker.h>
#include <ArduinoJson.h>

#include <stdlib.h>

#include "myluminancelistener.h"

class MyLuminance {
  private:
    int pin;
    int interval;
    int threshold;
    int lastValue;

    MyLuminanceListener *listener;
    TaskHandle_t taskHandle;
    Ticker ticker;

    void triggerGetTemp();
    void step();
    void doWork();
    void doWork(bool force);
    
    static void task(void * pvParameters);
    static void triggerStep(MyLuminance * mtemp);
  public:
    MyLuminance(const int pin, const int intervalMs, const int changeThreshold, MyLuminanceListener *listener);
    void start();
    void triggerEvent();
};

#endif
