#include "doublegateapp.h"

void DoubleGateApp::setup()
{
    gate1Open.setup();
    gate1Close.setup();
    gate1Stop.setup();
    gate1Contact.setup();

    gate2Open.setup();
    gate2Close.setup();
    gate2Stop.setup();
    gate2Contact.setup();

    openButton.setup();
}

bool DoubleGateApp::loop()
{
    gate1Open.loop();
    gate1Close.loop();
    gate1Stop.loop();
    gate1Contact.loop();

    gate2Open.loop();
    gate2Close.loop();
    gate2Stop.loop();
    gate2Contact.loop();

    openButton.loop();

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

    String action = doc["action"];

    if (action == "open" || action == "close" || action = "stop") {
        int door = doc["door"];

        if (door < 0 || door >= 1) {
            Serial.println("door out of range, ignore");
        }
    }


    /*


    int index = doc["button"];

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
