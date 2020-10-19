#include "doublegateapp.h"

void DoubleGateApp::setup()
{
    NetworkApp::setup();

    gate1Open.setup();
    gate1Close.setup();
    gate1Stop.setup();
    gate1Contact.setup();

    gate2Open.setup();
    gate2Close.setup();
    gate2Stop.setup();
    gate2Contact.setup();

    openButton.setup();
    led.setup();
    buzzer.setup();
    temperature.start();

    openButton.add(1000);
}

bool DoubleGateApp::loop()
{
    gate1Open.loop();
    gate1Close.loop();
    gate1Stop.loop();

    gate2Open.loop();
    gate2Close.loop();
    gate2Stop.loop();

    int btnValue = openButton.loop();
    led.loop();
    buzzer.loop();

    bool c1Changed = gate1Contact.loop();
    bool c2Changed = gate2Contact.loop();

    NetworkApp::loop();
    if (updateInterval.loop() || NetworkApp::isReconnected()) {
        sendContactStatus(0);
        sendContactStatus(1);
        temperature.triggerEvent();
    } else {
        if (c1Changed) {
            sendContactStatus(0);
        }

        if (c2Changed) {
            sendContactStatus(1);
        }
    }

    if (btnValue != 0 && lastButtonValue != btnValue) {
        String x(btnValue);
        Serial.println("open button set to: " + x);
        led.switchOn();
        if (lastButtonValue != btnValue && btnValue < 0) {
            buzzer.switchOn(50 * btnValue * -1);
        }
        lastButtonValue = btnValue;
    }

    return true;
}

void DoubleGateApp::sendContactStatus(int i)
{
    DynamicJsonDocument doc(1024);
    MyContactSwitch *contact = 0;
    if (i==0)
        contact = &gate1Contact;
    else
        contact = &gate2Contact;

    if (contact->isOpen())
        doc["value"] = "1";
    else
        doc["value"] = "0";

    this->updateStatus((String("door") + String(i)).c_str(), doc);
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

    if (action.equals("open") || action.equals("close") || action.equals("stop")) {
        int door = -1;
        if (doc.containsKey("door"))
            door = doc["door"];

        //door -1 means ALL doors
        if (door < -1 || door >= 1) {
            Serial.println("door out of range, ignore");
        }
    } else if (action.equals("buzz")) {
        int duration = 500; //in millis
        if (doc.containsKey("duration")) {
            duration = doc["duration"];
        }

        //TODO: sound buzzer for set duration
        buzzer.switchOn(duration);
    }

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

void DoubleGateApp::doStop()
{
    doStop(0);
    doStop(1);
}

void DoubleGateApp::doStop(int door)
{
    if (door == 0) {
        gate1Stop.switchOn();
    } else {
        gate2Stop.switchOn();
    }
}

void DoubleGateApp::doOpen(int door)
{
    if (door == 0) {
        gate1Open.switchOn();
    } else {
        gate2Open.switchOn();
    }
}

void DoubleGateApp::doClose(int door)
{
    if (door == 0) {
        gate1Close.switchOn();
    } else {
        gate2Close.switchOn();
    }
}

