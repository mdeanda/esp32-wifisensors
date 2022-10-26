#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <Arduino.h>

class MyTimer {
  private:
    unsigned long nextRun;

  public:
    MyTimer();

    void setup();
    bool loop(unsigned long now);
    void start(unsigned long now, unsigned long duration);
};

#endif
