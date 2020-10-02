#ifndef GARAGE_DOOR_APP_H
#define GARAGE_DOOR_APP_H

#include "networkapp.h"

#include <mybutton.h>
#include <mythrottle.h>
#include <mycontactswitch.h>
#include <mytimedswitch.h>
#include <mytemperature.h>
#include <mytemperaturelistener.h>
#include <myluminancelistener.h>
#include <myluminance.h>

#define LED_PIN 2
#define BUTTON_01 27 // ok
#define BUTTON_02 26 // ok
//#define BUTTON_03 25 // touch?
//#define BUTTON_04 33 // touch?
#define BUTTON_05 14 // ok
#define BUTTON_06 12 // ok
#define BUTTON_07 13 // ok

class GarageDoorApp : public NetworkApp, public MyTemperatureListener, public MyLuminanceListener {
  private:
//    MyThrottle throttle = MyThrottle(30000);
    std::vector<MyContactSwitch*> contactSwitches;
    std::vector<MyTimedSwitch*> doorButtons;
    std::vector<int> onQueue;
    MyTimedSwitch buzzer = MyTimedSwitch(4, 2000);
    MyThrottle updateInterval = MyThrottle(500000);
    MyTemperature temperature = MyTemperature(32, 30, this);
    MyLuminance luminance = MyLuminance(35, 1000, 200, this);

    bool runSequence = false;
    unsigned long seqStart = 0;

  public:
    GarageDoorApp() : NetworkApp() {};
    void setup();
    bool loop();
    void onMessage(char* topic, char* payload, unsigned int length);
    void onLuminance(int value);

    void onTemperature(
                float temperature,
                float humidity,
                float heatIndex,
                float dewPoint,
                String comfort);

  private:
    void toggleDoor();

    void sendContactStatus(int i);

};


#endif
