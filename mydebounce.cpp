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

void MyDebounce::loop()
{

    //bool down = digitalRead(pin) == HIGH;

    //if (stateDown) return;


}