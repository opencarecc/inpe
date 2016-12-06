#include "Arduino.h"

#include <LGPS.h>
#include <stdio.h>

#include "checkGPS.h"
#include "clock.h"

// initialize variables for GPS UTC time data reading
unsigned char *utc_date_time = 0;
char bufferDateTime[30] = {0,};

// initialize variables to convert GPS UTC time data into integers
//unsigned int year;
//unsigned int month;
//unsigned int day;
//unsigned int hour;
//unsigned int minute;
//unsigned int sec;
unsigned int year = 0;
unsigned int month = 0;
unsigned int day = 0;
unsigned int hour = 0;
unsigned int minute = 0;
unsigned int sec = 0;

const int timeZone = +1;  // add here the amount of hours difference of your time zone compared to GMT
                          // if not sure about your time zone try this http://everytimezone.com/

gclass::gclass(){}

/* ==== CHECK GPS ==== */
boolean gclass::checkGPS(){
  switch(LGPS.check_online()){  // check if GPS is online
    case 0:  // if GPS not connected return 0
      return 0;
      break;
    case 1:  // if GPS connected get the date
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

/* ==== GPS time conversion from char to int ==== */
void gclass::convertGPStimeToInt(){
  sprintf(bufferDateTime, "%d", utc_date_time[3]);  // get GPS UTC hour data
  hour = (atoi(bufferDateTime))+timeZone;  // convert GPS UTC hour data into integer (and add/subtract time zone)
  sprintf(bufferDateTime, "%d", utc_date_time[4]);
  minute = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[5]);
  sec = atoi(bufferDateTime);
      
//      Serial.print(hour);
//      Serial.print(" : ");
//      Serial.print(minute);
//      Serial.print(" : ");
//      Serial.println(sec);
}

gclass GPS = gclass();
