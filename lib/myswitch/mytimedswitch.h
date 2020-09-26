#ifndef MY_TIMED_SWITCH_H
#define MY_TIMED_SWITCH_H

#include "Arduino.h"

class MyTimedSwitch {
  private:
    int pin;
    uint8_t ON_VALUE = HIGH;
    uint8_t OFF_VALUE = LOW;
    unsigned long timeInMillis;
    unsigned long turnOffAfter = 0;

  public:
    MyTimedSwitch(const int pin, const unsigned long timeInMillis);
    MyTimedSwitch(const int pin, const unsigned long timeInMillis, bool reverse);
    void setup();
    bool loop();

    void switchOn();
    void switchOff();
};

#endif
