#ifndef MY_LCD_PROVIDER_H
#define MY_LCD_PROVIDER_H

#include <vector>

class MyLcdProvider {
    public:
    virtual std::vector<String> getSsOutput(int cols) = 0;
};

#endif
