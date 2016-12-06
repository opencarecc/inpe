#include "LDisplay.h"
#include "LGPS.h"
#include "stdio.h"

#include "clock.h"
#include "checkGPS.h"

// initialize flags to stop checking GPS
int checkGPSNumRuns = 0;
int checkGPSMaxRuns = 1;

int clockStart = 0;

void setup() {
  Serial.begin(115200);
  
  // initialize LCD
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(100);
  Lcd.screen_set(0x000000);
}

void loop() {
  if(checkGPSNumRuns<1){  // flag to keep/stop checking the GPS
    if(GPS.checkGPS()==1){    // if GPS is online and returning valid year
      GPS.convertGPStimeToInt();  // convert GPS UTC time data into integers

      Lcd.screen_set(0x000000);  // flush the screen
      
      clockStart = 1;  // flag to start the clock
      checkGPSNumRuns = 1;  // flag to stop checking GPS 
    } 
  } 
  
  if(clockStart == 0){  // if GPS not synced display placeholder
    Lcd.draw_font(10, 10, "Clock Sync...", 0x000000, 0xffffff, 20);
  }
  if(clockStart == 1){  // if GPS synced start clock
    clock.clock();
  }
  
  Lcd.draw_updata();
}
