#include "mytemperature.h"


MyTemperature::MyTemperature(const int pin, const int intervalSeconds, MyMqttWrapper * myMqttWrapper)
{
  dhtPin = pin;
  interval = intervalSeconds;
  mqtt = myMqttWrapper;
}


void MyTemperature::task(void * pvParameters) 
{
  while (1)
  {
    ((MyTemperature*)pvParameters) -> readTemperature();
    // Go to sleep again
    vTaskSuspend(NULL);
  }
  vTaskDelete(NULL);
}

void MyTemperature::triggerStep(MyTemperature * mtemp) {
  mtemp->step();
}

void MyTemperature::start()
{
  pinMode(this->dhtPin, INPUT);
  dht.setup(this->dhtPin, DHTesp::DHT22);
  delay(5000);
  
  xTaskCreate(
      MyTemperature::task,            // Function to implement the task
      "tempTask ",                    // Name of the task
      4000,                           // Stack size in words
      this,                           // Task input parameter
      5,                              // Priority of the task
      &tempTaskHandle);               // Task handle.

  if (tempTaskHandle == NULL) {
    Serial.println("Failed to start task for temperature update");
  } else {
    // Start update of environment data every interval seconds
    this->tempTicker.attach(this->interval, MyTemperature::triggerStep, this);
  }
}

void MyTemperature::step()
{
  xTaskResumeFromISR(this->tempTaskHandle);
}

void MyTemperature::readTemperature()
{
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT error status: " + String(dht.getStatusString()));
    return;
  }

  float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
  float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
  float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

  String comfortStatus;
  switch(cf) {
    case Comfort_OK:
      comfortStatus = "OK";
      break;
    case Comfort_TooHot:
      comfortStatus = "TooHot";
      break;
    case Comfort_TooCold:
      comfortStatus = "TooCold";
      break;
    case Comfort_TooDry:
      comfortStatus = "TooDry";
      break;
    case Comfort_TooHumid:
      comfortStatus = "TooHumid";
      break;
    case Comfort_HotAndHumid:
      comfortStatus = "HotAndHumid";
      break;
    case Comfort_HotAndDry:
      comfortStatus = "HotAndDry";
      break;
    case Comfort_ColdAndHumid:
      comfortStatus = "ColdAndHumid";
      break;
    case Comfort_ColdAndDry:
      comfortStatus = "ColdAndDry";
      break;
    default:
      comfortStatus = "Unknown";
      break;
  };

  StaticJsonDocument<400> doc;
  doc["temperature"] = String(newValues.temperature * 1.8 + 32);
  doc["humidity"] = String(newValues.humidity);
  doc["heat_index"] = String(heatIndex * 1.8 + 32);
  doc["dew_point"] = String(dewPoint * 1.8 + 32);
  doc["comfort"] = comfortStatus;

  last_temperature = newValues.temperature;
  last_humidity = newValues.humidity;
  last_comfort = comfortStatus;

  mqtt->publish(doc);
}

std::vector<String> MyTemperature::getSsOutput(int cols)
{
  std::vector<String> output;

  output.push_back("Temperature: " + String(last_temperature * 1.8 + 32));
  output.push_back("Humidity: " + String(last_humidity));
  output.push_back("Comfort: " + last_comfort);

  return output;
}


