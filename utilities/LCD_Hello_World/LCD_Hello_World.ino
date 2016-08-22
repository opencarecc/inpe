/*
  LCD Display Test for Rephone Kit
  
  Hello, this is a very simple sketch to test the
  Rephone's Xadow 1.54" Touchscreen Display
  
  HARDWARE:
  - Xadow GSM+BLE
  - Xadow 1.54" Touchscreen
  - FPC Cable 35 PIN
  - Lipo battery
  
  Alessandro Contini for http://opencare.cc/
  August 2016
*/

#include <LDisplay.h>    // LCD library

// delay variable
int time = 50; 

// array of hex values for colors
int color[4] = {0xffff00, 0xff007f, 0x33ffff, 0x00E271};

void setup()
{  
    // initalize LCD
    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(color[0]);    // set background color
}

void loop()
{
    // cycle through colors and print "Hello World!"
    // in the center of the display
    for(int i=0; i<4; i++){
      Lcd.screen_set(color[i]);
      Lcd.draw_font(68, 108, "Hello World!", color[i], 0);   
      delay(time);
    }
}

