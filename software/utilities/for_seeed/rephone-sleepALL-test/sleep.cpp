#include "Arduino.h"

#include "LDisplay.h"
#include "LGTouch.h"

#include "LTask.h"
#include "vmpwr.h"
#include "vmgsm.h"
#include "vmgsm_gprs.h"
#include "vmbt_cm.h"
#include "vmgps.h"

#include "sleep.h"

char EVENT = 0;
int X = 0;
int Y = 0;

unsigned long previousMillis = 0;

// set here (in milliseconds) the time interval to wait before going to sleep
const long sleepInterval = 1000*10; 

//int flag=0;

sleepClass::sleepClass(){}


void sleepClass::sleep(){
  // count time
  unsigned long currentMillis = millis();
  
//  if(flag==0){
  // if current time count is equal or bigger than the interval of time to wait before going to sleep
  if(currentMillis-previousMillis>=sleepInterval){
    // set backlight low
    Lcd.back_light_level(1);
    
    // send LCD to sleep
//    LTask.remoteCall((remote_call_ptr)vm_pwr_lcd_sleep_in, NULL);
    
//    LTask.remoteCall((remote_call_ptr)vm_gsm_switch_mode, NULL);
    
//    LTask.remoteCall((remote_call_ptr)vm_gsm_gprs_switch_mode, NULL);
    
//    LTask.remoteCall((remote_call_ptr)vm_bt_cm_switch_off, NULL);
    
//    LTask.remoteCall((remote_call_ptr)vm_gps_close, NULL);
    
    // save current time count to check for the time interval again later
    previousMillis = currentMillis;
//    flag=1;
//  }
}
  
//  if(flag==1){
//  if(currentMillis-previousMillis>=sleepInterval){
  
  // if there is a touch event
  if(Tp.Event_available()){
    Tp.Get_event_xy(&EVENT, &X, &Y);
    if (EVENT>0 && EVENT!=6 && X>=0 && Y>=0){
      // set backlight high
      Lcd.back_light_level(50);
      
      // bring back LCD from sleep
//      LTask.remoteCall((remote_call_ptr)vm_pwr_lcd_sleep_out, NULL);

//      LTask.remoteCall((remote_call_ptr)vm_gsm_switch_mode, NULL);

//      LTask.remoteCall((remote_call_ptr)vm_gsm_gprs_switch_mode, NULL);

//      LTask.remoteCall((remote_call_ptr)vm_bt_cm_switch_on, NULL);
    
//      LTask.remoteCall((remote_call_ptr)vm_gps_open, NULL);

      // save current count to wait for another whole time interval before going to sleep
      previousMillis = currentMillis;
//      flag=0;
    }
  }
//  }}
}

sleepClass sleep = sleepClass();
