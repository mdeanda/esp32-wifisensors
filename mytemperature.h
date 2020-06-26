#ifndef MY_TEMPERATURE
#define MY_TEMPERATURE

#include "DHTesp.h"
#include <Ticker.h>
#include <ArduinoJson.h>

#include "mymqttwrapper.h"

class MyTemperature {
  private:
    DHTesp dht;
    ComfortState cf;
    int dhtPin;
    int interval;
    TaskHandle_t tempTaskHandle;
    Ticker tempTicker;
    MyMqttWrapper * mqtt;

    void triggerGetTemp();
    void step();
    void readTemperature();

    static void task(void * pvParameters);
    static void triggerStep(MyTemperature * mtemp);
    
  public:
    MyTemperature(const int pin, const int intervalSeconds, MyMqttWrapper * myMqttWrapper);

    void start();
    
};

#endif
