#ifndef GARAGE_DOOR_APP_H
#define GARAGE_DOOR_APP_H

#include "networkapp.h"

#include <mythrottle.h>
#include <mycontactswitch.h>
#include <mytimedswitch.h>

#define LED_PIN 2

class GarageDoorApp : public NetworkApp {
  private:
//    MyThrottle throttle = MyThrottle(30000);
    MyContactSwitch doorContact = MyContactSwitch(12, 500);
    MyTimedSwitch doorButton = MyTimedSwitch(LED_PIN, 5000);
    
    MyContactSwitch auxDoorContact = MyContactSwitch(12, 500);

  public:
    GarageDoorApp() : NetworkApp() {};
    void setup();
    bool loop();

  private:
    void sendDoorStatus();
    void sendAuxDoorStatus();
    void sendSwitchStatus();

};


#endif
