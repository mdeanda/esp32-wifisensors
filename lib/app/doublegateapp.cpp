#include "doublegateapp.h"

void DoubleGateApp::setup()
{

}

bool DoubleGateApp::loop()
{
    return true;
}


void DoubleGateApp::onMessage(char* topic, char* payload, unsigned int length) 
{
    StaticJsonDocument<2048> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
        Serial.println("not json, ignore");
        return;
    };

    /*
    int index = doc["button"];
    if (index < 0 || index >= doorButtons.size()) {
        Serial.println("out of range, ignore");
    }

    Serial.println("add to queue: " + String(index));
    onQueue.push_back(index);
    runSequence = true;
    //*/

}

void DoubleGateApp::onTemperature(float temperature, float humidity, 
        float heatIndex, float dewPoint, String comfort)
{
    StaticJsonDocument<640> doc;
    doc["temperature"] = String(temperature);
    doc["humidity"] = String(humidity);
    doc["heat_index"] = String(heatIndex);
    doc["dew_point"] = String(dewPoint);
    doc["comfort"] = comfort;

    this->updateStatus("temperature", doc);
}

void DoubleGateApp::onLuminance(int value)
{
    StaticJsonDocument<256> doc;
    doc["luminance"] = String(value);

    this->updateStatus("luminance", doc);
}
