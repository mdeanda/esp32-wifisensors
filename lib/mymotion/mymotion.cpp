#include "mymotion.h"

MyMotion::MyMotion(const int pin, MyMqttWrapper * myMqttWrapper) 
{
    this->pin = pin;
    this->mqtt = myMqttWrapper;
    hasMotion = false;
}

void IRAM_ATTR motionInterrupt() {
  //portENTER_CRITICAL_ISR(&mux);
  //motionNow = true;
  //portEXIT_CRITICAL_ISR(&mux);
}

void MyMotion::start() 
{
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), motionInterrupt, RISING);

}

void MyMotion::loop()
{

  if (hasMotion) {
    //String msg = "\"motion\": \"interrupt\"";
    //publishMqtt(msg.c_str());
    portENTER_CRITICAL(&mux);
    hasMotion = false;
    portEXIT_CRITICAL(&mux);
  }
}


