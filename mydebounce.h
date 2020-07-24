#ifndef MY_DEBOUNCE_H
#define MY_DEBOUNCE_H

#include "Arduino.h"

class MyDebounce {
  private:
    int pin;
    bool stateDown;
    bool pressed;
    int threshold;
    unsigned long downTime;

  public:
    MyDebounce(const int pin, const int threshold);
    void start();
    bool loop();
};

#endif
