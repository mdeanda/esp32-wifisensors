#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "Arduino.h"
#include <vector>

struct MyButtonHold {
  int millis;
  int value;
};

using std::vector;

class MyButton {
  private:
    int pin;
    int timeInMillis;
    String label;

    bool pressed = false;
    unsigned long downStartTime;
    std::vector<MyButtonHold*> holdTimes;

  public:
    MyButton(const int pin, const int timeInMillis, const String label);

    void setup();

    /* returns 0 if not pressed or not released yet or not down long enough.
     * returns 1 if its down long enough, or the index of the added "time"
     */
    int loop();
    String getLabel();

    int add(const int timeInMillis);
};

#endif
