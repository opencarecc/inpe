/*  
  Hello, this sketch will create a txt log file in the
  storage of the Xadow GSM+BLE module containing the data
  stream coming from the sensors.
  The file will be named with date and time of
  its creation and will be stored in the root folder
  (accesible when in storage mode).
  
  Change the delay time and main function in the loop
  according to which sensor you want to track.
  
  The printAcc() function implements a check to detect fall
  and print it in the log as well, helpful for our "In pe'"
  project.
 
  HARDWARE:
  - Xadow GSM+BLE
  - Xadow GPS v2.2
  - Xadow GPSv2 module
  - Xadow SensorHub
  - FPC Cable 11 PIN
  - Lipo battery
  
  Alessandro Contini for http://opencare.cc/
  August 2016
*/

#include <LCFile.h>
#include <LGPS.h>
#include <stdio.h>
#include <LSensorHub.h>

// set a delay time to avoid flooding the log
int delayTime = 100;

// check number of loop runs
int numRuns = 0;
int maxRuns = 1;

// GPS values to name the file
unsigned char *utc_date_time = 0;
char fileName[21] = {0,};

// array to store chars for the heading of the txt file
char fileHeader[100] = {0,};

// Values to write
char writedata[40] = {0};  // will be used to print int the txt
unsigned long lightData = 0;  // will be used for Light values
long X = 0;  // will be used for X axis values
long Y = 0;  // will be used for Y axis values
long Z = 0;  // will be used for Z axis values

// array to store the fall detection values
char detected[100] = {0, };

void setup(){
//  Serial.begin(115200);
 
  // if GPS is online:
  // - create file
  // - name it with date and time
  // - add header in the txt file 
  if(LGPS.check_online())
      {
          utc_date_time = LGPS.get_utc_date_time();
          // check if the GPS is actually ready
          // by checking if the year displayed is 2016
          if(utc_date_time[0]==16){
            sprintf(fileName, "%d%d%d-%d%d%d.txt", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
            LFile.Create(fileName);
            sprintf(fileHeader, "File created: %d-%d-%d %d:%d:%d UTC +0\r\nDelay time: %d\r\n\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5], delayTime);
            LFile.Write(fileName, fileHeader);
          }          
      }  
}

void loop(){ 
  printAcc();
  delay(delayTime);
}

void printLight(){
  // write only once in the txt that light data will be printed
  if(numRuns<maxRuns){
    LFile.Write(fileName, "LIGHT DATA\r\n\r\n");
    numRuns++;
  }
  
  //get and write data
  LSensorHub.GetLighData(&lightData);  
  sprintf((char*)writedata, "Light value:\t%d\r\n", lightData);
  LFile.Write(fileName, writedata);
}

void printAcc(){
  // write only once in the txt that accelerometer
  // data will be printed
  if(numRuns<maxRuns){
    LFile.Write(fileName, "ACCELEROMETER DATA\r\n\r\n");
    numRuns++;
  }
  
  // get and print accelerometer data
  LSensorHub.GetAccData(&X, &Y, &Z);
  sprintf((char*)writedata, "X:%d\t\t\t Y:%d\t\t\t Z:%d\r\n", X,Y,Z);
  LFile.Write(fileName, writedata);
  
  // check fall detection
  if (X>=-50 && X<=50 && Y>=-50 && Y<=50 && Z>=-50 && Z<=50) {
    utc_date_time = LGPS.get_utc_date_time();
    sprintf(detected, "DETECTED!\r\non %d-%d-%d %d:%d:%d\r\nat LAT %c:%f LONG %c:%f\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4], utc_date_time[5], LGPS.get_ns(), LGPS.get_latitude(), LGPS.get_ew(), LGPS.get_longitude());
    LFile.Write(fileName, detected);
  }
}
