#include <LCFile.h>
#include <LGPS.h>
#include <stdio.h>
#include <LSensorHub.h>
#include <LDisplay.h>

// set a delay time to avoid flooding the log
int delayTime = 100;

// check number of loop runs
int numRuns = 0;
int maxRuns = 1;

// GPS values to name the file
unsigned char *utc_date_time = 0;
char month[2] = {0,};
char day[2] = {0,};

char fileName[21] = {0,};

// array to store chars for the heading of the txt file
char fileHeader[100] = {0,};

// Values to write
char writedata[40] = {0};  // will be used to print int the txt
unsigned long lightData = 0;  // will be used for Light values

//bool startBracket = FALSE;
//bool endBracket = FALSE;

unsigned int id = 0;

void setup() {
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
  
  // if GPS is online:
  // - create file
  // - name it with date and time
  // - add header in the txt file 
  if(LGPS.check_online())
      {
          utc_date_time = LGPS.get_utc_date_time();
          // check if the GPS is actually ready
          // by checking if the year displayed is 2016 or later
          if(utc_date_time[0]>=16){
            sprintf(fileName, "%02u%02u%02u-%02u%02u%02u.json", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
            LFile.Create(fileName);
//            sprintf(fileHeader, "File created: %d-%d-%d %d:%d:%d UTC +0\r\nDelay time: %d\r\n\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5], delayTime);
//            LFile.Write(fileName, fileHeader);
            Lcd.draw_font(10, 10, fileName, 0x000000, 0xffffff, 20);
            Lcd.draw_updata();
          }          
      }
}

void loop() {
  utc_date_time = LGPS.get_utc_date_time();
  printLight();
  delay(delayTime);
  Lcd.draw_font(10, 30, "Printing Light Value...", 0x000000, 0xffffff, 20);
  Lcd.draw_updata();
}

void printLight(){
  // write only once in the txt that light data will be printed
//  if(numRuns<maxRuns){
//    LFile.Write(fileName, "LIGHT DATA\r\n\r\n");
//    numRuns++;
//  }

//  if(startBracket==FALSE){
//    LFile.Write(fileName, "[");
//    startBracket=TRUE
//  }
  
  //get and write data
  LSensorHub.GetLighData(&lightData);  
//  sprintf((char*)writedata, "Light value:\t%d\r\n", lightData);
//  LFile.Write(fileName, writedata);
  
  id++;
  
  sprintf((char*)writedata, "{\r\n\"id\":\"%d\",\r\n\"lightValue\":\"%d\",\r\n\"date\":\"%02u%02u%02u\",\r\n\"time\":\"%02u%02u%02u\"\r\n},\r\n", id, lightData, utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
  LFile.Write(fileName, writedata);
//  {"id":"0","lightValue":"100","date":"161204","time":"151354"}
}
