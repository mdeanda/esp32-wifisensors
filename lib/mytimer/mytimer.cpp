#include "mytimer.h"

MyTimer::MyTimer()
{
  nextRun = 0;
}

void MyTimer::setup()
{
  nextRun = 0;
}

bool MyTimer::loop(unsigned long now)
{
  if (now > nextRun && nextRun > 0) {
    nextRun = 0;
    return true;
  }

  return false;
}

void MyTimer::start(unsigned long now, unsigned long duration)
{
  nextRun = now + duration;
}
