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
    this->switchOff();
}


void MyTimedSwitch::setup()
{
    pinMode(this->pin, OUTPUT);
}

bool MyTimedSwitch::loop()
{
    if (this->turnOffAfter != 0 && this->turnOffAfter < millis()) {
        this->switchOff();
    }
}

bool MyTimedSwitch::isOn()
{
    return digitalRead(this->pin) == ON_VALUE;
}

void MyTimedSwitch::switchOn()
{
    this->switchOn(this->timeInMillis);
}

void MyTimedSwitch::switchOn(const unsigned long timeInMillis)
{
    unsigned long now = millis();
    this->turnOffAfter = now + timeInMillis;

    Serial.println("ON  pin: " + String(this->pin) + " value: " + String(ON_VALUE));
    digitalWrite(this->pin, ON_VALUE);
}

void MyTimedSwitch::switchOff()
{
    Serial.println("OFF pin: " + String(this->pin) + " value: " + String(OFF_VALUE));
    digitalWrite(this->pin, OFF_VALUE);
    
    this->turnOffAfter = 0;
}
