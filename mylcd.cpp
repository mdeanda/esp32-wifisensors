#include "mylcd.h"

MyLcd::MyLcd(LiquidCrystal_I2C * lcd, const int rows, const int cols)
{
    this->lcd = lcd;
    this->rows = rows;
    this->cols = cols;
}

void MyLcd::loop()
{
    drawScreenSaver();
}

void MyLcd::drawScreenSaver()
{
    int next_line = ss_line;
    //int size = screenSaverContent.size();

    std::vector<String> lines;

    if (ss_line < 0 || scroll_time + 1500 < millis()) {
        //recreate output lines when we scroll
        for (MyLcd_SSItem * item : screenSaverContent) {
            //MyLcd_SSItem * item = screenSaverContent.at(el);
            if (item->type == STRING) {
                lines.push_back(item->text);
            } else if (item->type == PROVIDER) {
                std::vector<String> outputArray = item->provider->getSsOutput();
                
                for (String t : outputArray) {
                    lines.push_back(t);
                }
            } else {
                continue;
            }

        }

        next_line++;

        if (next_line >= lines.size() || next_line < 0) {
            next_line = 0;
        }

        if (ss_line != next_line)
            lcd->clear();
    } 
    
    if (ss_line != next_line) {
        ss_line = next_line;
        scroll_time = millis();

        int size = lines.size();
        for (int i=0; i<this->rows; i++) {
            String output = lines.at( (i+ss_line) % size);
            /*
            lcd->setCursor(0, i);
            lcd->print("                    ");
            lcd->setCursor(0, i);
            lcd->print(item->text);
            //*/
            //lcd->setCursor(0, i);
            //lcd->print(output);
            lcd->setCursor(0, i);
            lcd->print(output);
        }
    }
}

void MyLcd::addSsMessage(String msg)
{
    MyLcd_SSItem * item = new MyLcd_SSItem();
    item->text = msg;
    item->type = STRING;
    screenSaverContent.push_back(item);
}

void MyLcd::addSsMessageProvider(MyLcdProvider * provider)
{
    MyLcd_SSItem * item = new MyLcd_SSItem();
    item->provider = provider;
    item->type = MyLcd_SSItemType::PROVIDER;
    screenSaverContent.push_back(item);
}
