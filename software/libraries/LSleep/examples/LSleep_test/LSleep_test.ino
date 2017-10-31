/*
  Display sleep library for Rephone Kit
  
  Hello, this sketch will send the RePhone to sleep
  after a certain amount time.
  When in sleep mode, the following functions are called:
  - vm_pwr_lcd_sleep_in();
  - vm_gsm_cell_close();
  - vm_gsm_switch_mode(0, NULL);
  - vm_gsm_gprs_switch_mode(0);
  - vm_bt_cm_switch_off();

  Whenever the screen has been touched by the user
  the RePhone wakes up.
  
  The interval is set to 10 seconds.
  To change this value check "LSleep.cpp"
  
  Alessandro Contini for http://opencare.cc/
  Wayne Weng at https://www.seeed.cc/
  2016
*/

#include "LDisplay.h"  // include display libray
#include "LGTouch.h"  // include touch library

#include "LSleep.h"

long unsigned i;

void setup() 
{
    Serial.begin(115200);
    
    // initialize LCD display
    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(0x000000);
    
    // initialize touch
    Tp.Init();
}

void loop() 
{
    // draw stuff on screen
    Lcd.draw_font(10,10,"Sleep test", 0x000000, 0xffffff, 20);
    Lcd.draw_number(10,30,i++,0x000000,0xffffff,20);
    Lcd.draw_updata();

    // call sleep function
    lsleep.sleep();
}