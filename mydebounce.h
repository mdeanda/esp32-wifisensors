#ifndef MY_DEBOUNCE_H
#define MY_DEBOUNCE_H

#include "Arduino.h"

class MyDebounce {
  private:
    int pin;
    int threshold;

    bool stateDown;
    bool pressed;
    unsigned long downTime;

  public:
    MyDebounce(const int pin, const int threshold);
    void start();
    bool loop();
};

#endif
