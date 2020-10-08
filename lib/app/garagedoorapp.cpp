#include "garagedoorapp.h"

void GarageDoorApp::setup()
{
    NetworkApp::setup();

    //relay 1: 23, 19

    contactSwitches.push_back(new MyContactSwitch(BUTTON_06, 1000, true));
    contactSwitches.push_back(new MyContactSwitch(BUTTON_07, 1000, true));
    doorButtons.push_back(new MyTimedSwitch(23, 200, false));
    //doorButtons.push_back(new MyTimedSwitch(19, 200, !true));
    //doorButtons.push_back(new MyTimedSwitch(18, 200, !true));
    //doorButtons.push_back(new MyTimedSwitch(5, 200, !true));

    buzzer.setup();
    updateInterval.setup();

    int items = contactSwitches.size();
    for (int i=0; i<items; i++) {
        contactSwitches.at(i)->setup();
    }

    items = doorButtons.size();
    for (int i=0; i<items; i++) {
        doorButtons.at(i)->setup();
    }

    temperature.start();
    luminance.start();
}

bool GarageDoorApp::loop()
{
    NetworkApp::loop();
    buzzer.loop();

    int items = doorButtons.size();
    for (int i=0; i<items; i++) {
        doorButtons.at(i)->loop();
    }

    items = contactSwitches.size();
    if (updateInterval.loop() || NetworkApp::isReconnected()) {
        for (int i=0; i<items; i++) {
            sendContactStatus(i);
        }
        temperature.triggerEvent();
        luminance.triggerEvent();
    } else {
        for (int i=0; i<items; i++) {
            MyContactSwitch *doorSwitch = contactSwitches.at(i);
            if (doorSwitch->loop()) {
                buzzer.switchOn(200 * (i + 1));
                sendContactStatus(i);
            }
        }
    }

    if (this->runSequence) {
        this->toggleDoor();
    }

    return true;
}
void GarageDoorApp::sendContactStatus(int i)
{
    DynamicJsonDocument doc(1024);
    MyContactSwitch *doorSwitch = contactSwitches.at(i);

    if (doorSwitch->isOpen())
        doc["value"] = "1";
    else
        doc["value"] = "0";

    this->updateStatus((String("door") + String(i)).c_str(), doc);
}

void GarageDoorApp::toggleDoor()
{
    const unsigned long buzzDelay = 2000;

    unsigned long now = millis();
    if (this->seqStart == 0) {
        Serial.println("start");
        this->seqStart = now;
        buzzer.switchOn();
    }

    if (this->seqStart + buzzDelay < now) {
        int items = onQueue.size();
        for (int i=0; i<items; i++) {
            Serial.println("switch on button: " + String(i));
            doorButtons.at(i)->switchOn();
        }
        onQueue.clear();
        this->runSequence = false; //we're done
        this->seqStart = 0;
    }    
}

void GarageDoorApp::onMessage(char* topic, char* payload, unsigned int length) 
{
    StaticJsonDocument<2048> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
        Serial.println("not json, ignore");
        return;
    };

    int index = doc["button"];
    if (index < 0 || index >= doorButtons.size()) {
        Serial.println("out of range, ignore");
    }

    Serial.println("add to queue: " + String(index));
    onQueue.push_back(index);
    runSequence = true;

}

void GarageDoorApp::onTemperature(float temperature, float humidity, 
        float heatIndex, float dewPoint, String comfort)
{
    Serial.println("on temperature");

    StaticJsonDocument<640> doc;
    doc["temperature"] = String(temperature);
    doc["humidity"] = String(humidity);
    doc["heat_index"] = String(heatIndex);
    doc["dew_point"] = String(dewPoint);
    doc["comfort"] = comfort;

    this->updateStatus("temperature", doc);
}

void GarageDoorApp::onLuminance(int value)
{
    StaticJsonDocument<256> doc;
    doc["luminance"] = String(value);

    this->updateStatus("luminance", doc);
}
