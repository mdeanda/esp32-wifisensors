#include "mydebounce.h"

MyDebounce::MyDebounce(const int pin, const int threshold)
{
    this->pin = pin;
    this->threshold = threshold;
}

void MyDebounce::start()
{
    pinMode(pin, INPUT);
}

bool MyDebounce::loop()
{

    bool down = digitalRead(pin) == HIGH;

    if (!down) {
        stateDown = false;
        pressed = false;
        return false;
    }

    unsigned long now = millis();
    if (!stateDown) {
        stateDown = true;
        downTime = now;
        return false;
    }

    if (!pressed && now < downTime + (unsigned long) threshold ) {
        return true;
    } else {
        return false;
    }

}