#include "mybutton.h"

MyButton::MyButton(const int pin, const int timeInMillis, const String label)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    this->label = label;
}

void MyButton::setup()
{
    pinMode(pin, INPUT);
}

bool MyButton::loop()
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

    if (!pressed && now > downTime + (unsigned long) timeInMillis ) {
        //pressed long enough, stay pressed to allow 1 "press"
        pressed = true;
        return true;
    } else {
        return false;
    }

}

bool MyButton::isPressed()
{
    return pressed;
}

String MyButton::getLabel()
{
    return label;
}
