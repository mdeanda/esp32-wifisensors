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
        // not pressed, reset
        stateDown = false;
        pressed = false;
        return false;
    }

    unsigned long now = millis();
    if (!stateDown) {
        // newly pressed, debounce
        stateDown = true;
        downTime = now;
        return false;
    }

    if (!pressed && now < downTime + (unsigned long) threshold ) {
        //pressed long enough, stay pressed to allow 1 "press"
        pressed = true;
        return true;
    } else {
        return false;
    }

}