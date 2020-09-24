#ifndef MY_TIME_PROVIDER_H
#define MY_TIME_PROVIDER_H

#include <NTPClient.h>
#include "mylcdprovider.h"

namespace sensors {

class TimeProvider : public MyLcdProvider {
    private:
        NTPClient * ntp;

    public:
        TimeProvider(NTPClient * ntp);
        std::vector<String> getSsOutput(int cols);
};

}

#endif
