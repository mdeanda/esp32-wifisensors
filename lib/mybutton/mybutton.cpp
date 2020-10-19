#include "mybutton.h"

MyButton::MyButton(const int pin, const int timeInMillis, const String label)
{
    this->pin = pin;
    this->timeInMillis = timeInMillis;
    this->label = label;

    this->add(timeInMillis);
}

void MyButton::setup()
{
    pinMode(pin, INPUT);
}

int MyButton::add(const int timeInMillis)
{
    int value = holdTimes.size() + 1;
    MyButtonHold* hold = new MyButtonHold();
    hold->millis = timeInMillis;
    hold->value = value;
    holdTimes.push_back(hold);

    return value;
}

int MyButton::loop()
{
    int value = 0;
    unsigned long now = millis();
    bool wasPressed = pressed;
    pressed = digitalRead(pin) == HIGH;

    if (pressed && !wasPressed) {
        downStartTime = now;
    }

    if ((!pressed && wasPressed) || pressed) {
        //was released
        unsigned long dur = now - downStartTime;

        if (dur > timeInMillis) { //min down time
            //set value to 1
            //value = 1;

            int items = holdTimes.size();
            for (int i=0; i<items; i++) {
                MyButtonHold *hold = holdTimes.at(i);
                if (hold->millis < dur) {
                    value = hold->value;
                }
            }

            if (pressed) {
                //send negative value if still pressed
                value = -value;
            }
        }

    }
    

    return value;
}

String MyButton::getLabel()
{
    return label;
}
