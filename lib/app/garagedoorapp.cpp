#include "garagedoorapp.h"

void GarageDoorApp::setup()
{
    NetworkApp::setup();
}

bool GarageDoorApp::loop()
{
    NetworkApp::loop();
    doorButton.loop();

    if (NetworkApp::isReconnected()) {
        //notify of all statuses
        this->sendDoorStatus();
        this->sendAuxDoorStatus();        
        this->sendSwitchStatus();
    } else {
        if (doorContact.loop()) {
            this->sendDoorStatus();
        }

        if (auxDoorContact.loop()) {
            this->sendAuxDoorStatus();
        }
    }
    return true;
}

void GarageDoorApp::sendDoorStatus()
{
    DynamicJsonDocument doc(1024);
    if (doorContact.isOpen())
        doc["value"] = "1";
    else
        doc["value"] = "0";

    this->updateStatus("door", doc);
}

void GarageDoorApp::sendAuxDoorStatus()
{
    DynamicJsonDocument doc(1024);
    if (auxDoorContact.isOpen())
        doc["value"] = "1";
    else
        doc["value"] = "0";

    this->updateStatus("door2", doc);
}

void GarageDoorApp::sendSwitchStatus()
{
    DynamicJsonDocument doc(1024);
    if (doorButton.isOn())
        doc["value"] = "1";
    else
        doc["value"] = "0";

    this->updateStatus("button", doc);
}
