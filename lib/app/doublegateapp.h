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

class DoubleGateApp : public NetworkApp, public MyTemperatureListener {
  private:
    MyTimedSwitch gate1Open = MyTimedSwitch(RELAY_1_1, 200);
    MyTimedSwitch gate1Close = MyTimedSwitch(RELAY_1_2, 200);
    MyTimedSwitch gate1Stop = MyTimedSwitch(RELAY_1_3, 200);
    MyContactSwitch gate1Contact = MyContactSwitch(BUTTON_05, 1000, true);

    MyTimedSwitch gate2Open = MyTimedSwitch(RELAY_1_4, 200);
    MyTimedSwitch gate2Close = MyTimedSwitch(RELAY_2_1, 200);
    MyTimedSwitch gate2Stop = MyTimedSwitch(RELAY_2_2, 200);
    MyContactSwitch gate2Contact = MyContactSwitch(BUTTON_06, 1000, true);

    MyButton openButton = MyButton(BUTTON_07, 150, "button");
    MyTimedSwitch buzzer = MyTimedSwitch(RELAY_2_3, 500);
    MyTimedSwitch led = MyTimedSwitch(LED_BUILTIN, 1000);
    MyTemperature temperature = MyTemperature(DHT_PIN, 60, this);  
    
    MyThrottle updateInterval = MyThrottle(500000);

    int lastButtonValue = 0;

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

  private:
    void sendContactStatus(int i);
    void doStop();
    void doStop(int door);
    void doOpen(int door);
    void doClose(int door);

};

#endif
