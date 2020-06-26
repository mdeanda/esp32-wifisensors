#ifndef MY_LUMINANCE_H
#define MY_LUMINANCE_H

#include <Ticker.h>
#include <ArduinoJson.h>

#include "mymqttwrapper.h"

class MyLuminance {
  private:
    int pin;
    int interval;
    MyMqttWrapper * mqtt;
    TaskHandle_t taskHandle;
    Ticker ticker;

    void triggerGetTemp();
    void step();
    void doWork();
    
    static void task(void * pvParameters);
    static void triggerStep(MyLuminance * mtemp);
  public:
    MyLuminance(const int pin, const int intervalMs, MyMqttWrapper * myMqttWrapper);
    void start();
};

#endif
