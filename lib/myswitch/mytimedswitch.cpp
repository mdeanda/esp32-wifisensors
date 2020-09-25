#include "mytimedswitch.h"

MyTimedSwitch::MyTimedSwitch(const int pin, const unsigned long timeInMillis)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
}

MyTimedSwitch::MyTimedSwitch(const int pin, const unsigned long timeInMillis, bool reversed)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    uint8_t ON_VALUE = LOW;
    uint8_t OFF_VALUE = HIGH;
}


void MyTimedSwitch::setup()
{
    pinMode(pin, INPUT);
}

bool MyTimedSwitch::loop()
{
    if (turnOffAfter != 0 && turnOffAfter < millis()) {
        digitalWrite(this->pin, OFF_VALUE);

        turnOffAfter = 0;
    }
}

void MyTimedSwitch::switchOn()
{
    unsigned long now = millis();
    turnOffAfter = now + timeInMillis;

    digitalWrite(this->pin, ON_VALUE);
}
