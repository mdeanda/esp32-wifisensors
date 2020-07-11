#include "timeprovider.h"

namespace sensors {

TimeProvider::TimeProvider(NTPClient * ntp) {
    this->ntp = ntp;
}

std::vector<String> TimeProvider::getSsOutput(int cols)
{
    std::vector<String> output;

    String time = ntp->getFormattedTime();
    output.push_back(time);

    return output;
}

}
