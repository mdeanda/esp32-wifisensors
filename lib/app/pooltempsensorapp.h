#ifndef POOL_TEMP_SENSOR_APP_H
#define POOL_TEMP_SENSOR_APP_H

#include "networkapp.h"
#include <mytemperature.h>
#include <mytemperaturelistener.h>
#include <mythrottle.h>
#include "RunningMedian.h"

/**
 * Pool temperature sensor app to read temperature from a
 * 10k temperature sensor
 */
class PoolTempSensorApp : public NetworkApp, public MyTemperatureListener {
  private:
    const int SENSOR_PIN = 35;
    MyTemperature myTemperature = MyTemperature(32, 15, this);
    MyThrottle sampleThrottle = MyThrottle(3000);
    MyThrottle updateThrottle = MyThrottle(10000);
    RunningMedian samples = RunningMedian(15);

    int analogValue;
    float temperature;
    float humidity;
    float heatIndex;
    float dewPoint;
    String comfort;

  public:
    PoolTempSensorApp() : NetworkApp() {};
    void setup();
    bool loop();
    void onTemperature(
        float temperature,
        float humidity,
        float heatIndex,
        float dewPoint,
        String comfort);
  private:
    void sendUpdate();
};


#endif
