#include "mycontactswitch.h"

MyContactSwitch::MyContactSwitch(const int pin, const unsigned long timeInMillis, bool reversed)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    toggleOffAfter = 0;
    opened = false;

    if (reversed) {
        ON_VALUE = LOW;
        OFF_VALUE = HIGH;
    } else {
        ON_VALUE = HIGH;
        OFF_VALUE = LOW;

    }
}

void MyContactSwitch::setup()
{
    pinMode(pin, INPUT);
}

bool MyContactSwitch::loop()
{
    bool val = digitalRead(pin) == ON_VALUE;
    bool changed = false;

    if (val != this->opened) {
        unsigned long now = millis();
        if (now > this->toggleOffAfter) {
            this->opened = val;
            this->toggleOffAfter = 0;
        } else if (this->toggleOffAfter == 0) {
            this->toggleOffAfter = now + this->timeInMillis;
            changed = true;
        }
    } else if (this->toggleOffAfter != 0) {
        this->toggleOffAfter = 0;
    }

    return changed;
}

bool MyContactSwitch::isClosed()
{
    return !this->opened;
}


bool MyContactSwitch::isOpen()
{
    return this->opened;
}
