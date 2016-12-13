/*
  Battery Test for InPe'
  
  HARDWARE:
  - Xadow GSM+BLE
  - Xadow 1.54" Touchscreen
  - Xadow Audio
  - Xadow GPS
  - Xadow Basic Sensors
  - Lipo battery 3.7v 520mAh
  
  This sketch will log Battery data to a ".txt" file in the flash memory of the Xadow GSM+BLE.
  - GPS will be used only once to get UTC data
  - Battery level will be checked every 10 mins, to change this value check battCheckInterval
  - The LCD will go to sleep (lower back light) after 20 seconds, to change this value check "sleep.cpp"
  
  Alessandro Contini @ WeMake for http://opencare.cc/
  2016
*/

#include "LDisplay.h"  // include display libray
#include "LGTouch.h"  // include touch library

#include <LCFile.h>
#include <LGPS.h>
#include <stdio.h>

#include "LDateTime.h"

#include <LBattery.h>    // Battery library

#include "sleep.h"

// GPS values to name the file
unsigned char *utc_date_time = 0;
char fileName[100] = {0,};

// array to store chars for the heading of the txt file
char fileHeader[100] = {0,};

// array to temp store utc data to be converted into int
char bufferDateTime[30] = {0,};

// initialize variables to convert GPS UTC time data into integers
unsigned int year = 0;
unsigned int month = 0;
unsigned int day = 0;
unsigned int hour = 0;
unsigned int minute = 0;
unsigned int sec = 0;

datetimeInfo t;

const int timeZone = +1;  // add here the amount of hours difference of your time zone compared to GMT
                          // if not sure about your time zone try this http://everytimezone.com/

// array to store data to visualize clock on LCD
char time[10];

// array to store battery data to be written to txt file
char battery[100];

// variables to check battery status
int battLevel;
boolean battChargingPrinted = FALSE;
boolean battLevelPrinted = FALSE;

boolean battFirstTimeChecked = FALSE;

unsigned long prevBattCheckMillis = 0;
const long battCheckInterval = (1000*60)*10;  // battery check interval = 10 minutes

// create a function to draw a rectangle using inputs:
//    - X and Y coordinates for positioning of the first pixel
//    - width and height for rectangle size
//    - color
void draw_rect(int x, int y, int width, int height, uint32_t color){
  Lcd.draw_fill_rectangle(x,y,(x+width),(y+height),color);
}

void setup() {
  // initialize LCD display
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
  
  // initialize touch
  Tp.Init();
  
  Lcd.draw_font(10,10,"-- Battery test --", 0x000000, 0xffffff, 20);
  
  Lcd.draw_font(10,40,"GPS sync", 0x000000, 0xffffff, 20);
  
  // if GPS is online:
  // - convert UTC data into int and set time using DateTime library
  // - create file
  // - name it with date and time
  // - add header in the txt file 
  if(LGPS.check_online())
      {
          Lcd.draw_font(10,40,"GPS online", 0x000000, 0xffffff, 20);
          
          utc_date_time = LGPS.get_utc_date_time();
          // check if the GPS is actually ready
          // by checking if the year displayed is 2016 or later
          if(utc_date_time[0]>=16 && utc_date_time[0]!=80){
            Lcd.draw_font(10,40,"GPS READY  ", 0x000000, 0xffffff, 20);
            
            convertGPStimeToInt();
            dateTime_init();
            LDateTime.setTime(&t);
            
//            sprintf(fileName, "Battery_Test-%02u%02u%02u-%02u%02u%02u.txt", t.year, t.mon, t.day, t.hour, t.min, t.sec);
//            LFile.Create(fileName);
            sprintf(fileName, "Batt-%02u%02u%02u-%02u%02u%02u.txt", t.year, t.mon, t.day, t.hour, t.min, t.sec);
            LFile.Create(fileName);
            
            sprintf(fileHeader, "File created: %02u-%02u-%02u %02u:%02u:%02u\r\nUTC: %d\r\n\r\n", t.year, t.mon, t.day, t.hour, t.min, t.sec, timeZone);
            LFile.Write(fileName, fileHeader);
            Lcd.draw_font(10, 60, fileName, 0x000000, 0xffffff, 20);
          }          
      }
      
  Lcd.draw_updata();  // update LCD
}

void loop() {
  displayTime();
  batteryCheck(); 
  
  sleep.sleep();
  
  Lcd.draw_updata();  // update LCD
}

/* ==== CONVERT GPS UTC DATE AND TIME TO INT ==== */
void convertGPStimeToInt(){
  sprintf(bufferDateTime, "%d", utc_date_time[0]);
  year = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[1]);
  month = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[2]);
  day = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[3]);  // get GPS UTC hour data
  hour = (atoi(bufferDateTime))+timeZone;  // convert GPS UTC hour data into integer (and add/subtract time zone)
  sprintf(bufferDateTime, "%d", utc_date_time[4]);
  minute = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[5]);
  sec = atoi(bufferDateTime);
}

/* ==== INITIALIZE DATE AND TIME ==== */
void dateTime_init(){
  t.year  = year+2000;
  t.mon   = month;
  t.day   = day;
  t.hour  = hour;
  t.min   = minute;
  t.sec   = sec;
}

void displayTime(){
//  Serial.print("hour: "); Serial.print(hour); Serial.print("min: "); Serial.print(minute); Serial.print("sec: "); Serial.println(sec);
  LDateTime.getTime(&t);   
//  Lcd.draw_fill_rectangle(0,40,240,80,0x000000);
  sprintf(time, "%02u:%02u:%02u", t.hour, t.min, t.sec);
  Lcd.draw_font(10, 80, time, 0x000000, 0xffffff, 20);
}

void batteryCheck(){
  if(LBattery.isCharging()){            // if battery is charging
    draw_rect(0,120,240,20,0x000000);
    Lcd.draw_font(10, 120, "Battery is charging", 0x000000, 0xffffff, 20);
    if(battChargingPrinted==FALSE){    // only write to file once
      sprintf(battery, "[%02u-%02u-%02u %02u:%02u:%02u] Battery charging\r\n\r\n", t.year, t.mon, t.day, t.hour, t.min, t.sec);
      LFile.Write(fileName, battery);
      battChargingPrinted=TRUE;
    }  
  } else {                              // if battery is NOT charging
    if(battFirstTimeChecked==FALSE){
      battLevel = LBattery.level();
      
      draw_rect(0,120,240,20,0x000000);
      Lcd.draw_font(10, 120, "Battery level is:", 0x000000, 0xffffff, 20);
      Lcd.draw_number(150, 120, battLevel, 0x000000, 0xffffff, 20);
      
      sprintf(battery, "[%02u-%02u-%02u %02u:%02u:%02u] Battery level is %d\r\n\r\n", t.year, t.mon, t.day, t.hour, t.min, t.sec, battLevel);
      LFile.Write(fileName, battery);
      
      battFirstTimeChecked=TRUE;
    }
    
    unsigned long currBattCheckMillis = millis();
    if(currBattCheckMillis-prevBattCheckMillis>=battCheckInterval){
      battLevel = LBattery.level();
      
      draw_rect(0,120,240,20,0x000000);
      Lcd.draw_font(10, 120, "Battery level is:", 0x000000, 0xffffff, 20);
      Lcd.draw_number(150, 120, battLevel, 0x000000, 0xffffff, 20);
      
      sprintf(battery, "[%02u-%02u-%02u %02u:%02u:%02u] Battery level is %d\r\n\r\n", t.year, t.mon, t.day, t.hour, t.min, t.sec, battLevel);
      LFile.Write(fileName, battery);
      
      prevBattCheckMillis = currBattCheckMillis;
    }
  }
}


