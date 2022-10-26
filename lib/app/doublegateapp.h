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
    int STOP_VALUE = 1;
    int OPEN_VALUE = 999999; //gets set later
    int CLOSE_VALUE = 999999; //gets set later
    int SECRET_VALUE = 999999; //gets set later
    const int OPEN_HOLD_TIME = 1000;
    const int CLOSE_HOLD_TIME = 3000;
    const int SECRET_HOLD_TIME = 15000;

    MyTimedSwitch gate1Open = MyTimedSwitch(RELAY_1_1, 200);
    MyTimedSwitch gate1Close = MyTimedSwitch(RELAY_1_2, 200);
    MyTimedSwitch gate1Stop = MyTimedSwitch(RELAY_1_3, 200);
    MyContactSwitch gate1Contact = MyContactSwitch(BUTTON_06, 500, true);

    MyTimedSwitch gate2Open = MyTimedSwitch(RELAY_2_2, 200);
    MyTimedSwitch gate2Close = MyTimedSwitch(RELAY_2_1, 200);
    MyTimedSwitch gate2Stop = MyTimedSwitch(RELAY_1_4, 200);
    MyContactSwitch gate2Contact = MyContactSwitch(BUTTON_07, 500, true);

    MyButton openButton = MyButton(BUTTON_05, 150, "button"); //
    MyTimedSwitch buzzer = MyTimedSwitch(RELAY_2_3, 500);
    MyTimedSwitch led = MyTimedSwitch(LED_BUILTIN, 1000);
    MyTemperature temperature = MyTemperature(DHT_PIN, 300, this);  
    
    MyThrottle updateInterval = MyThrottle(500000);
    MyThrottle actionThrottle = MyThrottle(2000, true);

    int lastButtonValue = 0;
    bool test = false;

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
    void stopPressingStop();
    void doStop();
    void doStop(int door);
    void doOpen();
    void doOpen(int door);
    void doClose();
    void doClose(int door);
    void doSecret();

};

#endif
