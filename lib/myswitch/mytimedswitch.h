#ifndef MY_TIMED_SWITCH_H
#define MY_TIMED_SWITCH_H

#include "Arduino.h"

class MyTimedSwitch {
  private:
    int pin;
    int timeInMillis;

  public:
    MyTimedSwitch(const int pin, const int timeInMillis);
    void setup();
    bool loop();

    void switchOn();
};

#endif
