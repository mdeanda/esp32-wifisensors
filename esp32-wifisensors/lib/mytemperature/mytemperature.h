#ifndef MY_TEMPERATURE
#define MY_TEMPERATURE

#include <DHTesp.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <vector>

#include "mymqttwrapper.h"
#include "mylcdprovider.h"

class MyTemperature : public MyLcdProvider {
  private:
    DHTesp dht;
    ComfortState cf;
    int dhtPin;
    int interval;
    TaskHandle_t tempTaskHandle;
    Ticker tempTicker;
    MyMqttWrapper * mqtt;

    float last_temperature;
    float last_humidity;
    String last_comfort;

    float threshold_temperature = 0.6;
    float threshold_humidity = 1;

    void triggerGetTemp();
    void step();
    void readTemperature();

    static void task(void * pvParameters);
    static void triggerStep(MyTemperature * mtemp);
    
  public:
    MyTemperature(const int pin, const int intervalSeconds, MyMqttWrapper * myMqttWrapper);

    void start();

    std::vector<String> getSsOutput(int cols);
    
};

#endif
