#ifndef MY_TEMPERATURE_LISTENER_H
#define MY_TEMPERATURE_LISTENER_H

#include <vector>

class MyTemperatureListener {
    public:
    virtual void onTemperature(
            float temperature,
            float humidity,
            float heatIndex,
            float dewPoint,
            String comfort) = 0;
};

#endif
