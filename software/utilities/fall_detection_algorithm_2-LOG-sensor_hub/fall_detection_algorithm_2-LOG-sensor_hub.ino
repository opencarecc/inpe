#include <LSensorHub.h>

// initialize variables for accelerometer data
long accData1 = 0;  // will be used for X axis values
long accData2 = 0;  // will be used for Y axis values
long accData3 = 0;  // will be used for Z axis values
long prevData1 = 0;  // will be used for X axis values
long prevData2 = 0;  // will be used for Y axis values
long prevData3 = 0;  // will be used for Z axis values
long runningDelta = 0;

const long THRESH_FF = 250; // constant to check if free-fall is happening
const long FALLING_MINIMUM_TIME = 5;
const long FALLING_MAX_TIME = 200;
long freeFallCounter = 0;
bool accFallDetected = FALSE;
bool falling;

const long THRESH_IMPACT = 2000;  // constant to check if impact is happening
const long IMPACT_MAX_TIME = 3500;
bool impact;

const long THRESH_INACT = 187;
long INACT_MAX_TIME = 200;
bool inactivity;

int status = 0;
int fallingTime = 0;
int impactTime = 0;
int inactivityTime = 0;
bool fallDetected;

// sensor_loop stuff
// -------------------------------------
extern volatile long accData[128][3];
extern volatile int lastRead;
int firstRead = 0;
extern "C" {
  unsigned char startSensorLoop(void *);
}
// -------------------------------------

/* ==== LOG and DISPLAY STUFF ==== */
#include <LCFileExt.h>
#include <LDisplay.h>
#include <LGPS.h>
#include <stdio.h>
#include "LDateTime.h"

bool stopGPScheck = FALSE;
bool displayDateTime = FALSE;

// variables for vertical positioning of on display feedback 
int y = 150;
int y2 = 150;
int y3 = 0;

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




char I_DONT_KNOW_WHAT_THIS_ARRAY_IS_DOING_BUT_IF_I_REMOVE_IT_THE_SKETCH_GETS_STUCK_IN_THE_FALL_DETECT_LOOP_SEE_LINE_176[200];

int accActivity;
int accState;
int algState;

boolean sensorHubOnline;

/* |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */
int writtenToFileNum=0;

char dataToWrite[1000];
int dataWritten;

unsigned long writeCounter;

unsigned long microsToWrite[10000];
int sensorHubToWrite[10000];
long accDataToWrite[10000][3];
int activityToWrite[10000];
int accStateToWrite[10000];
int algStateToWrite[10000];
int fallTimeToWrite[10000];
int impactTimeToWrite[10000];
int inactTimeToWrite[10000];
int detectToWrite[10000];
/* |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| */

void draw_rect(int x, int y, int width, int height, uint32_t color){
  Lcd.draw_fill_rectangle(x,y,(x+width),(y+height),color);
}

void setup() {  
  delay(10000);
  
  Serial.begin(115200);
  
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
  LTask.remoteCall(startSensorLoop, NULL);
  
  Lcd.draw_font(10,10,"-- Fall detection test --", 0x000000, 0xffffff, 20);
  Lcd.draw_updata();  // update LCD
}

void loop() {
  /* ==== LOG FILE CREATION AND LCD FEEDBACK STUFF ==== */
  if(LSensorHub.check_on_line()==TRUE){
    sensorHubOnline=TRUE;
    
    if(stopGPScheck==FALSE){  // flag to keep/stop checking the GPS
      while(!GPSready()){
        Lcd.draw_font(10,40,"GPS sync  ", 0x000000, 0xffffff, 20);
        Lcd.draw_updata();
      }
      
      Lcd.draw_font(10,40,"GPS READY  ", 0x000000, 0xffffff, 20);
        
      // convert UTC data into int and set time using DateTime library
      convertGPStimeToInt();
      dateTime_init();
      LDateTime.setTime(&t);
      
      // create file and name it with date and time
      sprintf(fileName, "Fall-%02u%02u%02u-%02u%02u%02u.txt", t.year, t.mon, t.day, t.hour, t.min, t.sec);
      LFileExt.Create(fileName);
      Lcd.draw_font(10, 60, fileName, 0x000000, 0xffffff, 20); // and display filename on LCD
        
      // add header in the txt file 
      sprintf(fileHeader, "File created:\t%02u-%02u-%02u\t%02u:%02u:%02u\r\nUTC:\t%d\r\n\r\nMICROS\tSENSOR HUB ONLINE\tX\tY\tZ\tACC ACTIVITY\tACC STATE\tALG STATE S#\tFALLING TIME\tIMPACT TIME\tINACTIVITY TIME\tFALL DETECTED\tUSER INPUT\r\n\r\n", t.year, t.mon, t.day, t.hour, t.min, t.sec, timeZone);
      LFileExt.Write(fileHeader);
        
      displayDateTime = TRUE;  // flag to start the clock
      draw_rect(0,80,240,20,0x000000);
        
      Lcd.draw_font(10, 100, "File Written:", 0x000000, 0xffffff, 20);
        
      stopGPScheck = TRUE;  // flag to stop checking GPS
    }
    
    if(displayDateTime==FALSE){  // if GPS not synced display placeholder
      Lcd.draw_font(10, 80, "Clock Sync...", 0x000000, 0xffffff, 20);
    }
    if(displayDateTime==TRUE){  // if GPS synced start clock
      displayTime();
    }
  } else {
    sensorHubOnline=FALSE;
    Lcd.draw_font(0, 0, "Sensor Hub off-line", 0xffff00, 0, 20);
    
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
    Serial.println("Sensor Hub off-line");
  }
  /* ==== END OF LOG FILE CREATION etc... ==== */
  
  /* ==== FALL DETECTION LOOP ==== */  
  while (firstRead != lastRead) {
    accelLoop();
    accelStateMachine();  
    writeToFile();
  }
     
  Lcd.draw_updata();
}


/* ==== FALL DETECTION - accelerometer check ==== */
void accelLoop() {
  noInterrupts();
  // save 3 axis acc data to array
  accData1 = accData[firstRead][0];
  accData2 = accData[firstRead][1];
  accData3 = accData[firstRead][2];
  interrupts();
  firstRead++;
  if (firstRead == 128) firstRead = 0;

  // check if Free-fall
  if (accData1 >= -THRESH_FF && accData1 <= THRESH_FF &&
      accData2 >= -THRESH_FF && accData2 <= THRESH_FF &&
      accData3 >= -THRESH_FF && accData3 <= THRESH_FF)
  {
    accState = 1;  // save accelerometer state value for LOG
    accFallDetected = TRUE;
    
    freeFallCounter++;
    Lcd.draw_number(10, y, freeFallCounter, 0x000000, 0xffffff, 20);
    
    if (freeFallCounter >= FALLING_MINIMUM_TIME) {  // algorithm considers "Falling" only if falling for longer than FALLING_MINIMUM_TIME (5ms)
      falling = TRUE;
    }
  } else {
    accFallDetected = FALSE;    
    if (freeFallCounter > 0)  // for display format
      y += 15;
    freeFallCounter = 0;
    falling = FALSE;
  }

  // check if Impact
  if (accData1 <= -THRESH_IMPACT || accData1 >= THRESH_IMPACT ||
      accData2 <= -THRESH_IMPACT || accData2 >= THRESH_IMPACT ||
      accData3 <= -THRESH_IMPACT || accData3 >= THRESH_IMPACT)
  {
    accState = 2;  // save accelerometer state value for LOG
    Lcd.draw_font(50, y2, "IM", 0x000000, 0xffffff, 20);
    y2 += 10; 
    impact = TRUE;   
  } else {
    if (accFallDetected == FALSE){
      accState = 0;  // save accelerometer state value for LOG
    }
    if (impact) y += 10;
    impact = FALSE;
  }

  // check if Accelerometer Inactivity
  long delta = abs(accData1 - prevData1) + abs(accData2 - prevData2) + abs(accData3 - prevData3);
  runningDelta += -(runningDelta / 8) + delta / 8;
  
  if (runningDelta > 1000)
    runningDelta = 1000;
//  Lcd.draw_font(100, 150, "     ", 0x000000, 0xffffff, 20);
//  Lcd.draw_number(100, 150, runningDelta, 0x000000, 0xffffff, 20);
  if (runningDelta < THRESH_INACT)
  {
    accActivity = 0;  // save accelerometer state value for LOG   
    inactivity = TRUE;
    Lcd.draw_font(100, 180, "   ", 0x000000, 0xffffff, 20);
  } else {
    inactivity = FALSE;
    Lcd.draw_font(100, 180, "AC", 0x000000, 0xffffff, 20);
    accActivity = 1;  // save accelerometer state value for LOG
  }

  prevData1 = accData1;
  prevData2 = accData2;
  prevData3 = accData3;
}


/* ==== FALL DETECTION - state machine ==== */
void accelStateMachine(){  // will check timing for the stages of the fall detection algorithm
  switch (status) {
      case 0:
        algState = 0;  // save current algorithm state for LOG
        fallDetected = FALSE;  // save Fall Detected algorithm state for LOG
        
        if (falling) {  // if free-falling enter status 1
          Lcd.draw_font(25, y3, "S1", 0x000000, 0xffffff, 20);
          status = 1;
          fallingTime = 0;
        }
        break;
      case 1:
        algState = 1;  // save algorithm state for LOG
        
        if (fallingTime > FALLING_MAX_TIME) {  // if free-falling for too long go back to status 0
          y3 += 20;
          Lcd.draw_font(0, y3, "S0", 0x000000, 0xffffff, 20);
          fallingTime=0;
          status = 0;
          break;
        }
        if (!impact) {  // if not impact update free-falling counter
          fallingTime++;
        } else {  // else enter status 2
          Lcd.draw_font(50, y3, "S2", 0x000000, 0xffffff, 20);
          fallingTime=0;
          impactTime = 0;
          status = 2;
        }
        break;
      case 2:
        algState=2;
        
        if (impactTime > IMPACT_MAX_TIME) {  // if impact for too long go back to status 0
          y3 += 20;
          Lcd.draw_font(0, y3, "S0", 0x000000, 0xffffff, 20);
          impactTime=0;
          status = 0;
          break;
        }
        if (!inactivity) {  // if not inactivity update free-falling counter
          impactTime++;
        } else {  // else enter status 2
          Lcd.draw_font(100, y3, "S3", 0x000000, 0xffffff, 20); 
          impactTime=0;
          inactivityTime = 0;
          status = 3;
        }
        break;
      case 3:
        algState=3;
      
        if (inactivity) { // if inactivity update free-falling counter
          inactivityTime++;
        } else {  // else go back to status 0
          y3 += 20;
          Lcd.draw_font(0, y3, "S0", 0x000000, 0xffffff, 20);
          inactivityTime=0;
          status = 0;
          break;
        }
        
        if (inactivityTime > INACT_MAX_TIME) {  // if inactivity reaches INACT_MAX_TIME then all conditions are verified and fall is detected
          Lcd.draw_font(10, y3, "Fall Detected!", 0x000000, 0xffffff, 20);
          fallDetected = TRUE;
          y3 += 20;
          Lcd.draw_font(0, y3, "S0", 0x000000, 0xffffff, 20);
          inactivityTime=0;
          status = 0;
        }
        break;
  }
}


/* === WRITE LOG === */
void writeToFile(){  
  Serial.print(writeCounter);
  Serial.print("\t\t");
  // save values into arrays to be logged
  microsToWrite[writeCounter] = micros();
  sensorHubToWrite[writeCounter] = sensorHubOnline;
  accDataToWrite[writeCounter][0] = accData1;
  accDataToWrite[writeCounter][1] = accData2;
  accDataToWrite[writeCounter][2] = accData3;
  activityToWrite[writeCounter] = accActivity;
  accStateToWrite[writeCounter] = accState;
  algStateToWrite[writeCounter] = algState;
  fallTimeToWrite[writeCounter] = fallingTime;
  impactTimeToWrite[writeCounter] = impactTime;
  inactTimeToWrite[writeCounter] = inactivityTime;
  detectToWrite[writeCounter] = fallDetected;
  writeCounter++;
  
  if(writeCounter==1000) {  // log the first 1000 values
    for(int i=0; i<1000; i++){
      sprintf(dataToWrite, "%u\t%d\t%d\t%d\t%d\t%u\t%u\t%u\t%u\t%u\t%u\t%u\r\n", microsToWrite[i], sensorHubToWrite[i], accDataToWrite[i][0], accDataToWrite[i][1], accDataToWrite[i][2],activityToWrite[i],accStateToWrite[i],algStateToWrite[i],fallTimeToWrite[i],impactTimeToWrite[i],inactTimeToWrite[i],detectToWrite[i]);
      LFileExt.Write(dataToWrite); 
    }
    writeCounter=0;  // reset counter
    
    writtenToFileNum++;  // amount of times LOG has been written to file
  }
  
  Lcd.draw_number(125, 100, writtenToFileNum, 0x000000, 0xffffff, 20);
  Serial.println(writtenToFileNum);
}


/* ==== CHECK GPS ==== */
boolean GPSready(){
  switch(LGPS.check_online()){  // check if GPS is online
    case 0:  // if GPS not connected return 0
      return 0;
      break;
    case 1:  // if GPS connected get UTC date and time
      Lcd.draw_font(10,40,"GPS online", 0x000000, 0xffffff, 20);
      
      utc_date_time = LGPS.get_utc_date_time();
      
      if(utc_date_time[0]>=17 && utc_date_time[0]!=80){  // if year is 2016 or later and not *80 return 1
        return 1;
      } else {
        return 0;
      }
      break;
  }
}

/* ==== CONVERT GPS UTC DATE AND TIME TO INT ==== */
void convertGPStimeToInt(){
  sprintf(bufferDateTime, "%d", utc_date_time[0]);
  year = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[1]);
  month = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[2]);
  day = atoi(bufferDateTime);
  sprintf(bufferDateTime, "%d", utc_date_time[3]);
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
  LDateTime.getTime(&t);   
  sprintf(time, "%02u:%02u:%02u", t.hour, t.min, t.sec);
  Lcd.draw_font(10, 80, time, 0x000000, 0xffffff, 20);
}
