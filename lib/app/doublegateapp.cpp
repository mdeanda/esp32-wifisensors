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
    actionThrottle.setup();
    actionThrottle.start();

    stopTimer1.setup();
    stopTimer2.setup();

    OPEN_VALUE = openButton.add(OPEN_HOLD_TIME);
    CLOSE_VALUE = openButton.add(CLOSE_HOLD_TIME);
    SECRET_VALUE = openButton.add(SECRET_HOLD_TIME);
}

bool DoubleGateApp::loop()
{
    const unsigned long now = millis();

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

    if (stopTimer1.loop(now)) {
        gate1Stop.switchOn(now);
    }
    if (stopTimer2.loop(now)) {
        gate2Stop.switchOn(now);
    }

    NetworkApp::loop();
    if (updateInterval.loop() || NetworkApp::isReconnected()) {
        sendContactStatus(0);
        sendContactStatus(1);
        temperature.loop(now, true);
    } else {
        if (c1Changed) {
            sendContactStatus(0);
        }

        if (c2Changed) {
            sendContactStatus(1);
        }
        temperature.loop(now);
    }

    if (btnValue != 0 && lastButtonValue != btnValue) {
        Serial.print("open button set to: ");
        Serial.println(String(btnValue));
        led.switchOn(now);
        if (lastButtonValue != btnValue && btnValue < 0) {
            //button being held down still (btnValue<0)
            //TODO: implement double beep or something nicer
            int dur = abs(100 * btnValue * btnValue * -1);
            if (btnValue < -STOP_VALUE) {
                buzzer.switchOn(now, dur);
            }

            if (btnValue == -STOP_VALUE) {
                gate1Stop.switchOn(now, 1000);
                gate2Stop.switchOn(now, 1000);
            }
        }
        lastButtonValue = btnValue;

        if (btnValue > 0) {
            //button pressed and released
            //Serial.println("run action!");

            //run an action
            if (btnValue == this->STOP_VALUE) {
                doStop();
            } if (btnValue == this->OPEN_VALUE) {
                stopPressingStop();
                doOpen();
            } else if (btnValue == this->CLOSE_VALUE) {
                stopPressingStop();
                doClose();
            } else if (btnValue == this->SECRET_VALUE) {
                stopPressingStop();
                doSecret();
            }
        }
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
    unsigned long now = millis();
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

        if (action.equals("open")) {
            if (door < 0) {
                doOpen();
            } else {
                doOpen(door);
            }
        }
        if (action.equals("close")) {
            if (door < 0) {
                doClose();
            } else {
                doClose(door);
            }
        }
        if (action.equals("stop")) {
            if (door < 0) {
                doStop();
            } else {
                doStop(door);
            }
        }

    } else if (action.equals("buzz")) {
        int duration = 500; //in millis
        if (doc.containsKey("duration")) {
            duration = doc["duration"];
        }

        buzzer.switchOn(now, duration);
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

void DoubleGateApp::stopPressingStop()
{
    gate1Stop.switchOff();
    gate2Stop.switchOff();
}

void DoubleGateApp::doStop()
{
    doStop(0);
    doStop(1);
}

void DoubleGateApp::doStop(int door)
{
    const unsigned long now = millis();
    if (door == 0) {
        gate1Stop.switchOn(now);
    } else {
        gate2Stop.switchOn(now);
    }
}

void DoubleGateApp::doOpen()
{
    if (actionThrottle.loop()) {
        Serial.println("doOpen");
        actionThrottle.start();
        doOpen(0);
        doOpen(1);
    }
}

void DoubleGateApp::doOpen(int door)
{
    const unsigned long now = millis();
    if (door == 0) {
        gate1Open.switchOn(now);
        stopTimer1.start(now, GATE_OPEN_FAILSAFE_TIME);
    } else {
        gate2Open.switchOn(now);
        stopTimer2.start(now, GATE_OPEN_FAILSAFE_TIME);
    }
}

void DoubleGateApp::doClose()
{
    if (actionThrottle.loop()) {
        Serial.println("doClose");
        actionThrottle.start();
        doClose(0);
        doClose(1);
    }
}

void DoubleGateApp::doClose(int door)
{
    const unsigned long now = millis();
    if (door == 0) {
        gate1Close.switchOn(now);
    } else {
        gate2Close.switchOn(now);
    }
}

void DoubleGateApp::doSecret()
{
    Serial.println("doSecret");
    doClose();
    StaticJsonDocument<128> doc;

    this->updateStatus("secret", doc);
}
