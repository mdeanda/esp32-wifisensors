#ifndef MY_LCD
#define MY_LCD

#include <vector>
#include <LiquidCrystal_I2C.h>

struct MyLcd_SSItem {
    String text;
    struct MyLcd_SSItem * next;
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

    private:
        void drawScreenSaver();
};

#endif
