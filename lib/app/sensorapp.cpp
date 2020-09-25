#include "sensorapp.h"

void SensorApp::setup()
{
    NetworkApp::setup();
}

bool SensorApp::loop()
{
    bool connected = NetworkApp::loop();
    if (!connected) {
        //Serial.println("sensor - wait");
        //delay(1000);
    }

    //delay(100);

    return connected;
}
