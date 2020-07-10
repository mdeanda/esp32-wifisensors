#ifndef MY_LCD_PROVIDER_H
#define MY_LCD_PROVIDER_H

#include <vector>

class MyLcdProvider {
    public:
    virtual std::vector<String> getSsOutput() = 0;
};

#endif
