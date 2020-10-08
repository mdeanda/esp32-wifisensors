#ifndef DOUBLEGATE_APP_H
#define DOUBLEGATE_APP_H

#include "networkapp.h"
#include "pcb_buttons.h"

#include <mybutton.h>
#include <mythrottle.h>
#include <mycontactswitch.h>
#include <mytimedswitch.h>
#include <mytemperature.h>
#include <mytemperaturelistener.h>
#include <myluminancelistener.h>
#include <myluminance.h>

class DoubleGateApp : public NetworkApp, public MyTemperatureListener, public MyLuminanceListener {
  private:
    MyTimedSwitch gate1Open = MyTimedSwitch(4, 200);
    MyTimedSwitch gate1Close = MyTimedSwitch(4, 200);
    MyTimedSwitch gate1Stop = MyTimedSwitch(4, 200);
    MyContactSwitch gate1Contact = MyContactSwitch(BUTTON_06, 1000, true);

    MyTimedSwitch gate2Open = MyTimedSwitch(4, 200);
    MyTimedSwitch gate2Close = MyTimedSwitch(4, 200);
    MyTimedSwitch gate2Stop = MyTimedSwitch(4, 200);
    MyContactSwitch gate2Contact = MyContactSwitch(BUTTON_06, 1000, true);

    MyButton openButton = MyButton(BUTTON_01, 150, "button");

  public:
    DoubleGateApp() : NetworkApp() {};
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
};

#endif
