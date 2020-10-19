#include "mycontactswitch.h"

MyContactSwitch::MyContactSwitch(const int pin, const unsigned long timeInMillis, bool reversed)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    this->toggleOffAfter = 0;
    this->opened = false;

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
    opened = digitalRead(pin) == ON_VALUE;
}

bool MyContactSwitch::loop()
{
    bool val = digitalRead(pin) == ON_VALUE;
    bool orig = this->opened;

    if (val != this->opened) {
        unsigned long now = millis();
        if (this->toggleOffAfter == 0) {
            this->toggleOffAfter = now + this->timeInMillis;
        } else if (now > this->toggleOffAfter) {
            this->opened = val;
            this->toggleOffAfter = 0;
        }
    } else if (this->toggleOffAfter != 0) {
        this->toggleOffAfter = 0;
    }

    return this->opened != orig;
}

bool MyContactSwitch::isClosed()
{
    return !this->opened;
}


bool MyContactSwitch::isOpen()
{
    return this->opened;
}
