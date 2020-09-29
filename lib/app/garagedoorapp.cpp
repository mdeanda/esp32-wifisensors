#include "garagedoorapp.h"

void GarageDoorApp::setup()
{
    NetworkApp::setup();
}

bool GarageDoorApp::loop()
{
    bool connected = NetworkApp::loop();
    if (!connected) {
        //Serial.println("network disconnected");
        //TODO: track changes while offline or otherwise notify of entire state when back online
    }

    doorButton.loop();

    if (doorContact.loop()) {
        this->sendDoorStatus();
    }

    if (auxDoorContact.loop()) {
        this->sendAuxDoorStatus();
    }

    return true;
}

void GarageDoorApp::sendDoorStatus()
{
    if (doorContact.isOpen())
        this->updateStatus("door", "1");
    else
        this->updateStatus("door", "0");
}

void GarageDoorApp::sendAuxDoorStatus()
{
    if (auxDoorContact.isOpen())
        this->updateStatus("door2", "1");
    else
        this->updateStatus("door2", "0");
}
