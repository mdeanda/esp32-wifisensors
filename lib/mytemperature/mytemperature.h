#ifndef MY_TEMPERATURE
#define MY_TEMPERATURE

#include <DHTesp.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <vector>

#include "mylcdprovider.h"
#include "mytemperaturelistener.h"

class MyTemperature : public MyLcdProvider {
  private:
    DHTesp dht;
    ComfortState cf;
    int dhtPin;
    int interval;
    TaskHandle_t tempTaskHandle;
    Ticker tempTicker;

    float last_temperature;
    float last_humidity;
    String last_comfort;

    float threshold_temperature = 0.6;
    float threshold_humidity = 1;

    MyTemperatureListener *listener;

   
  public:
    MyTemperature(const int pin, const int intervalSeconds, MyTemperatureListener *listener);

    void start();
    void triggerEvent();

    std::vector<String> getSsOutput(int cols);
    
  private:
    void triggerGetTemp();
    void step();
    void readTemperature();
    void readTemperature(bool force);
    

    static void task(void * pvParameters);
    static void triggerStep(MyTemperature * mtemp);

};

#endif
