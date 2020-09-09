#ifndef MY_LCD_H
#define MY_LCD_H

#include <vector>
#include <LiquidCrystal_I2C.h>

#include "mylcdprovider.h"

enum MyLcd_SSItemType {
    STRING, PROVIDER
};

struct MyLcd_SSItem {
    String text;
    MyLcdProvider * provider;
    MyLcd_SSItemType type;
};


class MyLcd {
    private: 
        LiquidCrystal_I2C * lcd;
        int rows;
        int cols;
        std::vector<MyLcd_SSItem*> screenSaverContent;
    
        int ss_line = -1;
        unsigned long scroll_time;
        
    public:
        MyLcd(LiquidCrystal_I2C * lcd, const int rows, const int cols);

        /**
         * call this from loop() in main program
         */
        void loop();

        void addSsMessage(String msg);
        void addSsMessageProvider(MyLcdProvider * provider);

    private:
        void drawScreenSaver();
};

#endif
