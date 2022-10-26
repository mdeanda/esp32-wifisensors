#include "pooltempsensorapp.h"

void PoolTempSensorApp::setup()
{
    NetworkApp::setup();

    myTemperature.start();
    updateThrottle.setup();
    sampleThrottle.setup();
}

bool PoolTempSensorApp::loop()
{
    bool connected = NetworkApp::loop();
    unsigned long now = millis();
    if (!connected) {
        Serial.println("sensor - wait");
        delay(1000);
    } else {
      myTemperature.loop(now);
      if (sampleThrottle.loop()) {
        this->analogValue = analogRead(SENSOR_PIN);
        samples.add(this->analogValue);
      }

      if (updateThrottle.loop()) {
        this->sendUpdate();
      }
    }

    return connected;
}

void PoolTempSensorApp::sendUpdate()
{
    int rawValue = (int)samples.getAverage(5);
    float poolTemperature = (float)rawValue * 0.0352 + 8.76;

    /*
    Serial.print("last value vs average: ");
    Serial.print(this->analogValue);
    Serial.print(", ");
    Serial.print((int) samples.getAverage(5));
    Serial.println();
    // */

    StaticJsonDocument<640> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["heat_index"] = heatIndex;
    doc["dew_point"] = dewPoint;
    doc["comfort"] = comfort;
    doc["rawValue"] = rawValue;
    doc["poolTemperature"] = poolTemperature;

    this->updateStatus("value", doc);
}

void PoolTempSensorApp::onTemperature(float temperature, float humidity, 
        float heatIndex, float dewPoint, String comfort)
{

    this->temperature = temperature;
    this->humidity = humidity;
    this->heatIndex = heatIndex;
    this->dewPoint = dewPoint;
    this->comfort = comfort;
}
