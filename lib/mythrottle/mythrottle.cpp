#include "mythrottle.h"

MyThrottle::MyThrottle(const unsigned long interval)
{
    this->interval = interval;
    nextRun = 0;
}

void MyThrottle::setup()
{

}

bool MyThrottle::loop()
{
    unsigned long now = millis();
    if (now > nextRun) {
        nextRun = now + interval;
        return true;
    }

    return false;
}
