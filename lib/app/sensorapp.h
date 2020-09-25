#ifndef SENSOR_APP_H
#define SENSOR_APP_H

#include "networkapp.h"

class SensorApp : public NetworkApp {
  private:

  public:
    SensorApp() : NetworkApp() {};
    void setup();
    bool loop();

};


#endif
