#include "mytemperature.h"


MyTemperature::MyTemperature(const int pin, const unsigned long interval, MyTemperatureListener *listener)
{
  dhtPin = pin;
  this->interval = interval * 1000;
  this->listener = listener;

  nextRun = millis() + interval;
}

void MyTemperature::start()
{
  pinMode(this->dhtPin, INPUT);
  dht.setup(this->dhtPin, DHTesp::DHT22);
  //delay(3000);

  nextRun = millis() + 3000;
  
}

bool MyTemperature::loop(unsigned long now, bool force)
{
  if ((now > nextRun && nextRun != 0) || force) {
    //sometimes we can't read the sensor data, return false and dont update run time
    if (readTemperature(true)) {
      nextRun = now + interval;
      return true;
    }
  }

  return false;
}

bool MyTemperature::readTemperature(bool force)
{
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT error status: " + String(dht.getStatusString()));
    return false;
  }

  float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
  float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
  dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

  String comfortStatus;
  switch(cf) {
    case Comfort_OK:
      comfortStatus = "OK";
      break;
    case Comfort_TooHot:
      comfortStatus = "Too Hot";
      break;
    case Comfort_TooCold:
      comfortStatus = "Too Cold";
      break;
    case Comfort_TooDry:
      comfortStatus = "Too Dry";
      break;
    case Comfort_TooHumid:
      comfortStatus = "Too Humid";
      break;
    case Comfort_HotAndHumid:
      comfortStatus = "Hot And Humid";
      break;
    case Comfort_HotAndDry:
      comfortStatus = "Hot And Dry";
      break;
    case Comfort_ColdAndHumid:
      comfortStatus = "Cold And Humid";
      break;
    case Comfort_ColdAndDry:
      comfortStatus = "Cold And Dry";
      break;
    default:
      comfortStatus = "Unknown";
      break;
  };

  float diffTemp = abs(last_temperature - newValues.temperature);
  float diffHumi = abs(last_humidity - newValues.humidity);

  last_temperature = newValues.temperature;
  last_humidity = newValues.humidity;
  last_comfort = comfortStatus;

  if (diffTemp > threshold_temperature || diffHumi > threshold_humidity || force) {
    float temperature = newValues.temperature * 1.8 + 32;
    float humidity = newValues.humidity;
    heatIndex = heatIndex * 1.8 + 32;
    dewPoint = dewPoint * 1.8 + 32;

    if (this->listener) {
      this->listener->onTemperature(temperature, humidity, heatIndex, dewPoint, comfortStatus);
    }
  }
  
  return true;
}

std::vector<String> MyTemperature::getSsOutput(int cols)
{
  std::vector<String> output;

  output.push_back("Temperature: " + String(last_temperature * 1.8 + 32));
  output.push_back("Humidity: " + String(last_humidity));
  output.push_back("Comfort: " + last_comfort);

  return output;
}


