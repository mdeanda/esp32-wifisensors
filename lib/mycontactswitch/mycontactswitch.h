#ifndef MY_CONTACT_SWITCH_H
#define MY_CONTACT_SWITCH_H

#include "Arduino.h"

class MyContactSwitch {
  private:
    int pin;
    uint8_t ON_VALUE;
    uint8_t OFF_VALUE;
    unsigned long timeInMillis;
    unsigned long toggleOffAfter;
    bool opened;
    

  public:
    MyContactSwitch(const int pin, const unsigned long timeInMillis) : MyContactSwitch(pin, timeInMillis, false) {}
    MyContactSwitch(const int pin, const unsigned long timeInMillis, bool reverse);
    void setup();
    
    /** loop returns true if the value changed */
    bool loop();

    bool isOpen();
    bool isClosed();
};

#endif
