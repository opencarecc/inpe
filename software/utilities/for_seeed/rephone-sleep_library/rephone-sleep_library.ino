/*
  Display sleep library for Rephone Kit
  
  Hello, this sketch will lower the brightness of the Xadow 1.54" Touchscreen
  after a certain amount time. Whenever the screen has been touched by the user
  the brightness gets back to initial level.
  
  The interval is set to 10 seconds.
  To change this value check "sleep.cpp"
  
  Alessandro Contini for http://opencare.cc/
  2016
*/

#include "LDisplay.h"  // include display libray
#include "LGTouch.h"  // include touch library

#include "sleep.h"

void setup() {
  // initialize LCD display
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
  
  // initialize touch
  Tp.Init();
}

void loop() {
  Lcd.draw_font(10,10,"Sleep test", 0x000000, 0xffffff);
  sleep.sleep();
}
