#include "mythrottle.h"

MyThrottle::MyThrottle(const unsigned long interval, const bool manualStart)
{
  this->interval = interval;
  this->manualStart = manualStart;
  nextRun = 0;
}

void MyThrottle::setup()
{
  if (!manualStart) {
    this->start();
  }
}

bool MyThrottle::loop()
{
  unsigned long now = millis();
  if (now > nextRun && nextRun != 0) {
    Serial.print(".");
    if (!manualStart) {
      nextRun = now + interval;
    } else {
      Serial.print("o");
      nextRun = 0;
    }
    return true;
  }

  return false;
}

void MyThrottle::start()
{
  Serial.println("start throttle");
  unsigned long now = millis();
  nextRun = now + interval;
}
