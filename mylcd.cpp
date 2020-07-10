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
    int size = screenSaverContent.size();

    if (ss_line < 0 || scroll_time + 1000 < millis()) {
        next_line++;

        if (next_line >= size || next_line < 0) {
            next_line = 0;
        }

        if (ss_line != next_line)
            lcd->clear();
    } 
    
    if (ss_line != next_line) {
        ss_line = next_line;
        scroll_time = millis();

        for (int i=0; i<this->rows; i++) {
            MyLcd_SSItem * item = screenSaverContent.at( (i+ss_line) % size);
            String output = item->text;// + "                    ";
            //output = output.substring(0, cols); //TODO: make sure we padded enough
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
    screenSaverContent.push_back(item);
}
