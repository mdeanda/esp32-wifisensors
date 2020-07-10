#include "myluminance.h"

MyLuminance::MyLuminance(const int pin, const int intervalMs, MyMqttWrapper * myMqttWrapper)
{
  this->pin = pin;
  interval = intervalMs;
  mqtt = myMqttWrapper;
}

void MyLuminance::start()
{
  pinMode(this->pin, INPUT);
  
  xTaskCreate(
      MyLuminance::task,              // Function to implement the task
      "luminanceTask ",               // Name of the task
      4000,                           // Stack size in words
      this,                           // Task input parameter
      5,                              // Priority of the task
      &taskHandle);                   // Task handle.

  if (taskHandle == NULL) {
    Serial.println("Failed to start task for luminance update");
  } else {
    // Start update of environment data every interval seconds
    this->ticker.attach_ms(this->interval, MyLuminance::triggerStep, this);
  }
}

void MyLuminance::task(void * pvParameters) 
{
  while (1)
  {
    ((MyLuminance*)pvParameters) -> doWork();
    // Go to sleep again
    vTaskSuspend(NULL);
  }
  vTaskDelete(NULL);
}

void MyLuminance::triggerStep(MyLuminance * ptr) {
  ptr->step();
}

void MyLuminance::step()
{
  xTaskResumeFromISR(this->taskHandle);
}

void MyLuminance::doWork()
{
  int value = analogRead(pin);

  StaticJsonDocument<400> doc;
  doc["luminance"] = String(value);

  mqtt->publish(doc);
}
