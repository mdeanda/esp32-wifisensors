#ifndef THROTTLE_H
#define THROTTLE_H

#include <Arduino.h>

class MyThrottle {
  private:
    unsigned long interval;
    unsigned long nextRun;
    bool manualStart;

  public:
    MyThrottle(const unsigned long interval) : MyThrottle(interval, false) {};
    MyThrottle(const unsigned long interval, const bool manualStart);

    void setup();
    bool loop();
    void start();
};

#endif
