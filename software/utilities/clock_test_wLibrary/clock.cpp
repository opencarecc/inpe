#include "Arduino.h"

#include <LDisplay.h>

#include "clock.h"

// initialize variables to run the clock
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 1000;

char sec_char[2] = {0,};

cclass::cclass(){}

/* ==== CLOCK ==== */
void cclass::clock(){
  // count seconds without delay
  currentMillis = millis();
  if(currentMillis-previousMillis>=interval){
    sec++;
    previousMillis = currentMillis; 
//    Serial.print(hour);
//    Serial.print(" : ");
//    Serial.print(minute);
//    Serial.print(" : ");
//    Serial.println(sec);

    if(sec>=60){  // count seconds into minutes and reset
      sec=0;
      minute++;
    }  
    if(minute>=60){  // count minutes into hours and reset
      minute=0;
      hour++;
    }
    if(hour>=24){  // reset hours at midnight
      hour=0;
    }
    
    // display time (if value is single digit add zero behind for formatting reasons)
    if(hour<10){
      Lcd.draw_number(10, 10, 0, 0x000000, 0xffffff, 20);
      Lcd.draw_number(20, 10, hour, 0x000000, 0xffffff, 20);
    } else {
      Lcd.draw_number(10, 10, hour, 0x000000, 0xffffff, 20);
    }
    
    Lcd.draw_font(36, 10, ":", 0x000000, 0xffffff, 20);
    
    if(minute<10){
      Lcd.draw_number(47, 10, 0, 0x000000, 0xffffff, 20);
      Lcd.draw_number(57, 10, minute, 0x000000, 0xffffff, 20);
    } else {
      Lcd.draw_number(47, 10, minute, 0x000000, 0xffffff, 20);
    }
    
    Lcd.draw_font(71, 10, ":", 0x000000, 0xffffff, 20);
    
    if(sec<10){
      Lcd.draw_number(81, 10, 0, 0x000000, 0xffffff, 20);
      Lcd.draw_number(91, 10, sec, 0x000000, 0xffffff, 20);
    } else {
      Lcd.draw_number(81, 10, sec, 0x000000, 0xffffff, 20);
    }
    
//    itoa(sec, sec_char, 10);
//    if(sec<10){
//      Lcd.draw_font(81, 10, "0", 0x000000, 0xffffff);
//      Lcd.draw_font(91, 10, sec_char, 0x000000, 0xffffff);
//      Lcd.draw_font(101, 10, " ", 0x000000, 0xffffff);
//    } else {
//      Lcd.draw_font(81, 10, sec_char, 0x000000, 0xffffff);
//    }    
  } 
}

cclass clock = cclass();
