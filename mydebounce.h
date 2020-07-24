#ifndef MY_DEBOUNCE_H
#define MY_DEBOUNCE_H

#include "Arduino.h"

class MyDebounce {
  private:
    int pin;
    bool stateDown;
    int threshold;

  public:
    MyDebounce(const int pin, const int threshold);
    void start();
    void loop();
};

#endif
