#ifndef GARAGE_DOOR_APP_H
#define GARAGE_DOOR_APP_H

#include "networkapp.h"

#include <mythrottle.h>

class GarageDoorApp : public NetworkApp {
  private:
    MyThrottle throttle = MyThrottle(30000);

  public:
    GarageDoorApp() : NetworkApp() {};
    void setup();
    bool loop();

};


#endif
