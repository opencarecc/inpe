#include "LDisplay.h"
#include "LGPS.h"
#include "stdio.h"
#include "LDateTime.h"

bool stopGPScheck = FALSE;
bool displayDateTime = FALSE;

// initialize variables for GPS UTC time data reading
unsigned char *utc_date_time = 0;
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

char date[11];
char time[10];

void setup() {
  Serial.begin(115200);
  
  // initialize LCD
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
}

void loop() {
  if(stopGPScheck==FALSE){  // flag to keep/stop checking the GPS
    if(GPSready()==TRUE){    // if GPS is online and returning valid year
      convertGPStimeToInt();
      dateTime_init();
      LDateTime.setTime(&t);

      Lcd.screen_set(0x000000);  // flush the screen
      
      displayDateTime = TRUE;  // flag to start the clock
      stopGPScheck = TRUE;  // flag to stop checking GPS 
    } 
  } 
  
  if(displayDateTime==FALSE){  // if GPS not synced display placeholder
    Lcd.draw_font(10, 10, "Clock Sync...", 0x000000, 0xffffff, 20);
  }
  if(displayDateTime==TRUE){  // if GPS synced start clock
    displayDate();
    displayTime();
  }
  
  Lcd.draw_updata();
}

/* ==== CHECK GPS ==== */
boolean GPSready(){
  switch(LGPS.check_online()){  // check if GPS is online
    case 0:  // if GPS not connected return 0
      return 0;
      break;
    case 1:  // if GPS connected get UTC date and time
      utc_date_time = LGPS.get_utc_date_time();
      Serial.println("checking GPS");
      sprintf(bufferDateTime, "%d", utc_date_time[0]);  // get GPS UTC year data
      year = atoi(bufferDateTime);  // convert GPS UTC year data into integer
      Serial.print("year: ");
      Serial.println(year);
      
      if(year>=16 && year!=80){  // if year is 2016 or later and not *80 return 1
        return 1;
      } else {
        return 0;
      }
      break;
  }
}

/* ==== CONVERT GPS UTC DATE AND TIME TO INT ==== */
void convertGPStimeToInt(){
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

void displayDate(){
//  Serial.print("year: "); Serial.print(year); Serial.print("month: "); Serial.print(month); Serial.print("day: "); Serial.println(day);
  LDateTime.getTime(&t);
  sprintf(date, "%02u-%02u-%02u", t.year, t.mon, t.day);
  Lcd.draw_fill_rectangle(0,0,240,30,0x000000);
  Lcd.draw_font(10, 10, date, 0x000000, 0xffffff, 20);
}

void displayTime(){
//  Serial.print("hour: "); Serial.print(hour); Serial.print("min: "); Serial.print(minute); Serial.print("sec: "); Serial.println(sec);
  LDateTime.getTime(&t);   
  Lcd.draw_fill_rectangle(0,40,240,80,0x000000);
  sprintf(time, "%02u:%02u:%02u", t.hour, t.min, t.sec);
  Lcd.draw_font(10, 40, time, 0x000000, 0xffffff, 40);
}
