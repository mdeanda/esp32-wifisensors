#include "mytimedswitch.h"

MyTimedSwitch::MyTimedSwitch(const int pin, const unsigned long timeInMillis, bool reversed)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    this->turnOffAfter = 0;

    if (reversed) {
        ON_VALUE = LOW;
        OFF_VALUE = HIGH;
    } else {
        ON_VALUE = HIGH;
        OFF_VALUE = LOW;
    }
}


void MyTimedSwitch::setup()
{
    pinMode(pin, OUTPUT);
}

bool MyTimedSwitch::loop()
{
    if (turnOffAfter != 0 && turnOffAfter < millis()) {
        this->switchOff();
    }
}

bool MyTimedSwitch::isOn()
{
    return digitalRead(this->pin) == ON_VALUE;
}

void MyTimedSwitch::switchOn()
{
    unsigned long now = millis();
    turnOffAfter = now + timeInMillis;

    digitalWrite(this->pin, ON_VALUE);
}

void MyTimedSwitch::switchOff()
{
    digitalWrite(this->pin, OFF_VALUE);
    
    turnOffAfter = 0;
}
