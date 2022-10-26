#ifndef MY_TIMED_SWITCH_H
#define MY_TIMED_SWITCH_H

#include "Arduino.h"

class MyTimedSwitch {
  private:
    int pin;
    uint8_t ON_VALUE;
    uint8_t OFF_VALUE;
    unsigned long timeInMillis;
    unsigned long turnOffAfter;

  public:
    MyTimedSwitch(const int pin, const unsigned long timeInMillis) : MyTimedSwitch(pin, timeInMillis, false) {}
    MyTimedSwitch(const int pin, const unsigned long timeInMillis, bool reverse);
    void setup();
    bool loop();

    bool isOn();
    void switchOn(const unsigned long now);
    void switchOn(const unsigned long now, const unsigned long timeInMillis);
    void switchOff();
};

#endif
