#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "Arduino.h"

class MyButton {
  private:
    int pin;
    int timeInMillis;
    String label;

    bool stateDown;
    bool pressed;
    unsigned long downTime;

  public:
    MyButton(const int pin, const int timeInMillis, const String label);
    void setup();
    bool loop();
    bool isPressed();
    String getLabel();
};

#endif
