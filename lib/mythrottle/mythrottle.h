#ifndef THROTTLE_H
#define THROTTLE_H

#include <Arduino.h>

class MyThrottle {
  private:
    unsigned long interval;
    unsigned long nextRun;

  public:
    MyThrottle(const unsigned long interval);

    void setup();
    bool loop();
};

#endif
