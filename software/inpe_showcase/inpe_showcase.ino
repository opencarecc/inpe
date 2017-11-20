/*
  This sketch is a fake version of InPe.
  It would turn useful when showing how it works to
  people at events without asking them to necessarily
  fall to the ground ;)
  
  HARDWARE:
  - Xadow GSM+BLE (with Antenna!)
  - Xadow 1.54" Touchscreen
  - Xadow Audio
  - Xadow GPS
  - Xadow Basic Sensors
  - Lipo battery 3.7v 520mAh
  
  TO DO:
  - move fall trigger to somewhere more visible
  - add sleep library
  - add check to SMS function to only write the position when GPS available
  - add option to sync time with GPS time when available
  - better UI (LDisplayExt)
  
  Alessandro Contini @ WeMake for http://opencare.cc/
  2017
*/


#include <LDisplay.h>
#include <LSensorHub.h>
#include <LAudio.h>
#include <LGTouch.h>
#include <LGPS.h>
#include <stdio.h>
#include <LCheckSIM.h>
#include <LGSM.h>
#include <LBattery.h>
//#include <LCFile.h>
#include <LDateTime.h>
#include "LSleep.h"

// initialize variables for GSM
char *phoneNumber = "enter-phone-number";

// initialize variables for Date and Time
// set time
unsigned int year     = 2017;
unsigned int month    = 11;
unsigned int day      = 6;
unsigned int hour     = 18;
unsigned int minute   = 7;
unsigned int sec      = 22;

datetimeInfo t;

char date[11];
char time[10];

// initialize variables for GPS UTC time data reading
unsigned char *utc_date_time = 0;
char bufferDateTime[30] = {0,};

unsigned int GPSyear      = 0;
unsigned int GPSmonth     = 0;
unsigned int GPSday       = 0;
unsigned int GPShour      = 0;
unsigned int GPSminute    = 0;
unsigned int GPSsec       = 0;

const int timeZone = +1;  // add here the amount of hours difference of your time zone compared to GMT
                          // if not sure about your time zone try this http://everytimezone.com/

boolean GPSDateTimeSynced = false;
boolean GPStimeConverted = false;

// initialize variables for audio module
#define    FILE_NAME    "rephone_audio.mp3"
int volume = 6;  // 0 is min, 6 is max volume

// initialize variables for touch event
char EVENT = 0;
int X = 0;
int Y = 0;

// initialize variables for accelerometer data
long accData1 = 0;  // will be used for X axis values
long accData2 = 0;  // will be used for Y axis values
long accData3 = 0;  // will be used for Z axis values

boolean fallFlag = false;   // alarm screen flag

boolean systemFlag = false; // system check flag

//boolean testFlag = false;   // test mode flag

// string to save the SMS SENT or NOT SENT data
char *SMSsentFeedback;
boolean smsSent = false;  // check to send SMS only once

// string to save the System Ready or System Error data
char *systemCheck;

// string to save the Sensor Hub Status (ON/OFF) data 
char *sensorHubStatus;
// string to save the GPS (ON/OFF) data 
char *GPSState;
// string to save the GPS (Sync/Ready/Error) data 
char *GPSStatus;

// string to save the SIM Status (OK/Error) data 
char *SIMStatus;

// set min and max coordinates for easier positioning on LCD screen
int xInit = 0;
int yInit = 0;
int xFinal = 240;
int yFinal = 240;

// create a function to draw a rectangle using inputs:
//    - X and Y coordinates for positioning of the first pixel
//    - width and height for rectangle size
void draw_rect(int x, int y, int width, int height, uint32_t color) {
  Lcd.draw_fill_rectangle(x, y, (x + width), (y + height), color);
}

/* TEST MODE - FILE LOG STUFF */
// set a delay time to avoid flooding the log
int delayTime = 100;
// check number of loop runs
int numAccPrintRuns = 0;
int maxAccPrintRuns = 1;
// array to store chars for the file name
char fileName[21] = {0,};
// array to store chars for the heading of the txt file
char fileHeader[100] = {0,};
char writedata[40] = {0};  // will be used to print int the txt
// array to store the fall detection values
char detected[100] = {0, };
// var to store the status of Test Mode (0=OFF/1=ON) 
int testModeStatus = 0;

//////////////////////////////////////////////////////////
///////////////////////   SETUP   ////////////////////////
//////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);

  // Initialize Audio module
  LAudio.begin();
  LAudio.setVolume(volume);
  // play audio file to notify audio system is ready and device is ON
  LAudio.playFile(storageFlash, (char*)FILE_NAME);  

  // initialize touch
  Tp.Init();

  // initialize LCD
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(100);
  Lcd.screen_set(0x000000);

  // initialize Date and Time
  time_init();
  LDateTime.setTime(&t);
}

//////////////////////////////////////////////////////////
///////////////////////   LOOP   /////////////////////////
//////////////////////////////////////////////////////////

void loop()
{
  // check modules are connected and ready
  checkSensorHub();
  checkGPS();
  checkSIM();

  // show visual feedback if system ready or not
  checkSystem();

  // build home screen
  homeScreen();
  
  Lcd.draw_updata();  // update screen 
}


//////////////////////////////////////////////////////////
////////////////   CHECK ACCELEROMETER   /////////////////
//////////////////////////////////////////////////////////
/* Check if Accelerometer is working */
boolean checkSensorHub() {
  switch(LSensorHub.check_on_line()){
    /* if Sensor Hub is not connected */
    case 0:                                           
      sensorHubStatus = "OFF  ";
      return false;
      break;
    /* if Sensor Hub is connected */
    case 1:                                           
      LSensorHub.GetAccData(&accData1, &accData2, &accData3);
      /* and if you are receiving values different than zero from the Accelerometer */  
      if (accData1!=0 && accData2!=0 && accData3!=0) {
        sensorHubStatus = "ON   ";
        return true;
        break;
      } else {
        sensorHubStatus = "ERROR";
        return false;
        break;
      }
      break;
  }
}


//////////////////////////////////////////////////////////
/////////////////////   CHECK GPS   //////////////////////
//////////////////////////////////////////////////////////
/* Check if GPS is working */
boolean checkGPS() {  
  switch(LGPS.check_online()){
    /* if GPS module is not connected */
    case 0:
      GPSState = "OFF";
      Serial.println("GPS is OFF");
      return false;
      break;
    /* if GPS module is connected */
    case 1:
      GPSState = "ON  ";
      /* if GPS Status is V(=navigation receiver warning) */
      if (LGPS.get_status()==86) {
          GPSStatus = "Sync  ";
          Serial.println("GPS is V");
      }
      /* if GPS Status is A(=Valid position) */
      else if (LGPS.get_status()==65) {
        GPSStatus = "Ready";
        Serial.println("GPS is A");
        return true;        
        break;
      } else {
        GPSStatus = "Error";
        Serial.println("GPS is ERROR");
      }
      break;
  }  
}


//////////////////////////////////////////////////////////
/////////////////////   CHECK SIM   //////////////////////
//////////////////////////////////////////////////////////
/* Check if a SIM card is inserted */
boolean checkSIM() {
  switch(LCheckSIM.isCheck()){
    /* if SIM is not inserted */
    case 0:
      SIMStatus = "Error";
      return false;
      break;
    /* if SIM is inserted */
    case 1:
      SIMStatus = "OK   ";
      return true;
      break;
  }
}


//////////////////////////////////////////////////////////
///////////////////   CHECK SYSTEM   /////////////////////
//////////////////////////////////////////////////////////
/* 
   If Xadow modules are connected and receiving
   correct data show visual feedback
*/
boolean checkSystem(){
  if(checkSensorHub()==true && checkGPS()==true && checkSIM()==true){
    systemCheck = "Ready :)";
    // if GPS available sync time with GPS UTC time
    GPStimeSync();      
    return true;
  } else {
    systemCheck = "Error :(";
    return false;
  }
}


//////////////////////////////////////////////////////////
////////////////////   HOME SCREEN   /////////////////////
//////////////////////////////////////////////////////////
void homeScreen(){
  Serial.println("Into Home Screen");
  
  // flush content by creating a new background
  draw_rect(0,30,240,210,0x000000);
  
  while(systemFlag==false && fallFlag==false){
    Serial.println("Into Home Screen While");
    createHeader();
    displayDate();
    displayTime(); 
    
    Lcd.draw_font(87, 125, "InPe is", 0, 0xffffff, 20);
    if(checkSystem()==true){
      // font color green
      draw_rect(50,130,150,40,0x000000);
      Lcd.draw_font(89, 150, systemCheck, 0, 0x00ff00, 20);
    } else {
      // font color red
      draw_rect(50,150,147,40,0x000000);
      Lcd.draw_font(85, 150, systemCheck, 0, 0xff0000, 20);
      Lcd.draw_font(50, 170, "Check System >", 0, 0xff0000, 20);
    }
    
    if (Tp.Event_available()) {
      Tp.Get_event_xy(&EVENT, &X, &Y);
      triggerCheckScreen();    
      if(triggerFall()==true){
        Serial.println("Fall triggered");
        fallFlag = true;
        doTheAlarm();
      }  
    }
//    testMode();

    Lcd.draw_updata();
  }   
}

/* ==== UI HEADER ==== */
void createHeader(){
  draw_rect(0, 0, 240, 30, 0xffffff);
  batteryLevel();
}
void displayDate(){
  LDateTime.getTime(&t);
  sprintf(date, "%02u-%02u-%02u", t.year, t.mon, t.day);
  Lcd.draw_font(8, 8, date, 0xffffff, 0x000000, 18);
}
void displayTime(){
  LDateTime.getTime(&t);
  sprintf(time, "%02u:%02u", t.hour, t.min);
  Lcd.draw_font(55, 65, time, 0, 0xffffff, 50);
  sprintf(time, ":%02u", t.sec);
  Lcd.draw_font(182, 85, time, 0, 0xffffff, 18);
}

/* ==== BATTERY ==== */
void batteryLevel(){
  // draw battery icon
  draw_rect((xFinal)-48,5,35,20,0x000000);
  draw_rect((xFinal)-13,8,4,14,0x000000);
  draw_rect((xFinal)-45,8,29,14,0xffffff);
    
  // check if battery is charging
  if(LBattery.isCharging()){
    // draw charging icon
    // this is pretty experimental, need to find a better algorithm
    float k = 6;
    float h = 21;
    for(unsigned char x=0;x<20;x++){
      k=k-0.7;
      for(float y=0;y<k;y++){
        Lcd.draw_point(x+(xFinal-32),y+14,0x000000);
      }
    }
    for(unsigned char x=0;x<20;x++){
      h=h-0.7;
      for(float y=12;y>h;y--){
        Lcd.draw_point(x+(xFinal-49),y+2,0x000000);
      }
    }
  } else {
    // fill battery icon from L to R according to battery level
    if (LBattery.level()==0){
        draw_rect((xFinal)-43,10,3,10,0xff0000);
    }
    if (LBattery.level()==33){
      draw_rect((xFinal)-43,10,10,10,0xff0000);
    }
    if (LBattery.level()==66){
      draw_rect((xFinal)-43,10,18,10,0x000000);
    }
    if (LBattery.level()==100){
      draw_rect((xFinal)-43,10,25,10,0x000000);
    }
  }
}


//////////////////////////////////////////////////////////
////////////////   SYSTEM CHECK SCREEN   /////////////////
//////////////////////////////////////////////////////////
/* ==== TRIGGER TO SYSTEM CHECK SCREEN ==== */
void triggerCheckScreen(){
  if (EVENT>0 && EVENT!=2 && X>=40 && X<=210 && Y>=140 && Y<=180) {
    systemFlag = true;
    systemCheckScreen();     
  }
}

/* ==== SYSTEM CHECK SCREEN ==== */
void systemCheckScreen() {  
  // flush content
  draw_rect(0,30,240,210,0x000000);

  while(systemFlag == 1){
    Serial.println("Into System Check Screen While");
    
    createHeader();
    displayDate();

    checkSensorHub();
    checkGPS();
    checkSIM();
    
    displaySensorHubValues();
    displayGPSData();
    displaySIM();
    
    draw_rect((xFinal)-87, 205, 87, 40, 0xffffff);
    Lcd.draw_font((xFinal)-78, 215, "< Back", 0xffffff, 0x000000, 20);
    
    // if a touch event is detected
    if (Tp.Event_available()) {
      Tp.Get_event_xy(&EVENT, &X, &Y);
      if (EVENT > 0 && EVENT!=2 && X>=(xFinal)-87 &&  Y >= 220) {
        systemFlag = 0;
        homeScreen();     
      }
    }
    Lcd.draw_updata();
  }
}

/* ==== DISPLAY SIM DATA ==== */
void displaySIM(){
  Lcd.draw_font(10, 180, "SIM Status:", 0, 0xffffff, 20);
  if(LCheckSIM.isCheck()){
    Lcd.draw_font(120, 180, SIMStatus, 0, 0xffffff, 20);
  } else {
    Lcd.draw_font(120, 180, SIMStatus, 0, 0xff0000, 20);
  }
}

/* ==== DISPLAY GPS DATA ==== */
void displayGPSData() {
  Lcd.draw_font(10, 110, "GPS:", 0, 0xffffff, 20);
  if (LGPS.check_online()){
    // if GPS is connected display GPSState in white
    Lcd.draw_font(60, 110, GPSState, 0, 0xffffff, 20);
  } else {
    Lcd.draw_font(60, 110, GPSState, 0, 0xff0000, 20);
  }  
  char GPSDataBuffer[50] = {0,};
  sprintf(GPSDataBuffer, "Status: %c\r\n", LGPS.get_status());
  Lcd.draw_font(120, 110, GPSDataBuffer, 0, 0xffffff, 20);
  sprintf(GPSDataBuffer, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
  Lcd.draw_font(10, 130, GPSDataBuffer, 0, 0xffffff, 20);
  sprintf(GPSDataBuffer, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());
  Lcd.draw_font(10, 150, GPSDataBuffer, 0, 0xffffff, 20);
}

/* ==== DISPLAY SENSOR HUB VALUES ==== */
 void displaySensorHubValues() {
    // get data from sensors
    LSensorHub.GetAccData(&accData1, &accData2, &accData3);
    Lcd.draw_font(10, 40, "Accelerometer:", 0, 0xffffff, 20);
    if(checkSensorHub()){
      // if SensorHub is connected display sensorHubStatus in white
      Lcd.draw_font(150, 40, sensorHubStatus, 0, 0xffffff, 20);
    } else {
      // if SensorHub is connected display sensorHubStatus in red
      Lcd.draw_font(150, 40, sensorHubStatus, 0, 0xff0000, 20);
    }
/* print Acc X value to LCD */
    Lcd.draw_font(10, 60, "X:", 0, 0xffffff, 20);
    if(accData1 > 0){
        Lcd.draw_font(10, 80, "+", 0, 0xffffff, 20);
        Lcd.draw_number(20, 80, accData1, 0, 0xffffff, 20);
    } else {
        accData1 ^= 0xffffffff;
        accData1 += 1;
        Lcd.draw_font(10, 80, "-", 0, 0xffffff, 20);
        Lcd.draw_number(20, 80, accData1, 0, 0xffffff, 20);
    }

/* print Acc Y value to LCD */
    Lcd.draw_font((xFinal/2)-20, 60, "Y:", 0, 0xffffff, 20);
    if(accData2 > 0){
        Lcd.draw_font((xFinal/2)-20, 80, "+", 0, 0xffffff, 20);
        Lcd.draw_number((xFinal/2)-10, 80, accData2, 0, 0xffffff, 20);
    } else {
        accData2 ^= 0xffffffff;
        accData2 += 1;
        Lcd.draw_font((xFinal/2)-20, 80, "-", 0, 0xffffff, 20);
        Lcd.draw_number((xFinal/2)-10, 80, accData2, 0, 0xffffff, 20);
    }

/* print Acc Z value to LCD */
    Lcd.draw_font((xFinal)-50, 60, "Z:", 0, 0xffffff, 20);
    if(accData3 > 0){
        Lcd.draw_font((xFinal)-50, 80, "+", 0, 0xffffff, 20);
        Lcd.draw_number((xFinal)-40, 80, accData3, 0, 0xffffff, 20);
    } else {
        accData3 ^= 0xffffffff;
        accData3 += 1;
        Lcd.draw_font((xFinal)-50, 80, "-", 0, 0xffffff, 20);
        Lcd.draw_number((xFinal)-40, 80, accData3, 0, 0xffffff, 20);
    }
}


//////////////////////////////////////////////////////////
///////////////////////   ALARM   ////////////////////////
//////////////////////////////////////////////////////////
/* ==== FALL TRIGGER ==== */
boolean triggerFall() {
  if (EVENT>0 && EVENT!=2 && X>=0 && X<=240 && Y>=0 && Y<=40) {   
//      sprintf(detected, "DETECTED!\r\non %d-%d-%d %d:%d:%d\r\nat LAT %c:%f LONG %c:%f\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4], utc_date_time[5], LGPS.get_ns(), LGPS.get_latitude(), LGPS.get_ew(), LGPS.get_longitude());
//      LFile.Write(fileName, detected);
    return true; 
  } else {
    return false;
  }
}

/* ==== START ALARM ==== */
void doTheAlarm() {
  Serial.println("Into Alarm");

  // switch screen color
  draw_rect(0,0,240,240,0xff007f);
  
  while(fallFlag==true){
  Serial.println("Into Alarm While");
  
  // display triggered text
  Lcd.draw_font(40, 40, "FALL DETECTED!", 0xff007f, 0, 20);
  //  Lcd.draw_font(80, 80, "Calling", 0xff007f, 0);
  //  Lcd.draw_font(50, 80, SMSsentFeedback, 0xff007f, 0);
  Lcd.draw_font(50, 80, "  SMS sent to:", 0xff007f, 0, 20);
  Lcd.draw_font(40, 100, phoneNumber, 0xff007f, 0, 20);
  
  // play audio file
  Serial.println("Play the audio");
  AudioStatus status;
  LAudio.playFile(storageFlash, (char*)FILE_NAME);

  delay(1000);
  
  // if not into STATUS MODE
//  if (testFlag==0){
//    Lcd.draw_font(60, 200, "touch to stop", 0xff007f, 0, 20);
//  }
//  if (testFlag==1){
//    Lcd.draw_font(60, 140, "Confirm fall?", 0xff007f, 0, 20);
//    draw_rect((xFinal/2)-80,170,60,60,0x000000);
//    Lcd.draw_font((xFinal/2)-70, 190, "YES", 0x000000, 0xff007f, 20);
//    draw_rect((xFinal/2)+20,170,60,60,0x000000);
//    Lcd.draw_font((xFinal/2)+35, 190, "NO", 0x000000, 0xff007f, 20);
//  }

  Lcd.draw_font(60, 200, "touch to stop", 0xff007f, 0, 20);

  doSMS();

  delay(1000);
  
  // check touch event and exit triggered state if LCD is touched
  if (Tp.Event_available()) {
//    if (testFlag==0){
      Tp.Get_event_xy(&EVENT, &X, &Y);
      if (EVENT > 0 && EVENT!=2) {
        
        fallFlag = false;
        // close call
        LVoiceCall.hangCall();

        smsSent=false;
        
        homeScreen();
      }
//    } else
//    if (testFlag==1){
//      Tp.Get_event_xy(&EVENT, &X, &Y);
//      // if press Confirm YES
//      if (EVENT > 0 && EVENT!=2 && X>=(xFinal/2)-80 && X<=(xFinal/2)-20 && Y>=170 && Y<=230) {
//        draw_rect((xFinal/2)-80,170,60,60,0xffffff);
//        Lcd.draw_font((xFinal/2)-70, 190, "YES", 0xffffff, 0xff007f, 20);
//        LFile.Write(fileName, "[CONFIRMED]");
//        testFlag=0;
//        flag = 0;
//        homeScreen();
//      } else
//      if (EVENT > 0 && EVENT!=2 && X>=(xFinal/2)+20 && X<=(xFinal/2)+80 && Y>=170 && Y<=230) {
//        draw_rect((xFinal/2)+20,170,60,60,0xffffff);
//        Lcd.draw_font((xFinal/2)+35, 190, "NO", 0xffffff, 0xff007f, 20);
//        LFile.Write(fileName, "[NOT CONFIRMED]");
//        testFlag=0;
//        flag = 0;
//        homeScreen();
//      }
//    }
  }

Lcd.draw_updata();
  }
}

/* ==== SEND SMS TO SELECTED NUMBER ==== */
void doSMS() {
  if(smsSent == false){
    char SMScontent[159] = {0,};

    if (checkGPS() == true){
      sprintf(SMScontent, "HELP! I fell here http://maps.google.com/maps?q=%f,%f", LGPS.get_latitude(), LGPS.get_longitude());
      Serial.print("SMS content: ");
      Serial.println(SMScontent);
    } else {    
      sprintf(SMScontent, "HELP! I have fallen");
      Serial.print("SMS content: ");
      Serial.println(SMScontent);
    }
  
    Serial.println("Sending SMS:");
//    while(!LSMS.ready()){}
    if(LSMS.ready()){
      Serial.println("SMS ready is ok");
  
      LSMS.beginSMS(phoneNumber);
      LSMS.print(SMScontent);
      
      if(LSMS.endSMS()){
        Serial.println("SMS is sent");
        SMSsentFeedback = "  SMS sent to:";
        smsSent = true;
      } else {
        Serial.println("SMS send fail");
        SMSsentFeedback = "SMS failed to:";
      }
    } else {
      Serial.println("SMS not ready");
      SMSsentFeedback = "SMS Error to:";
    }
  }
}

/* ==== START A CALL TO SELECTED NUMBER ==== */
void doTheCall() {
//Serial.println("do the call");
  // check if SIM is ready
  if (LCheckSIM.isCheck() == true) {
    Serial.println("Start call");
    // start call
    LVoiceCall.voiceCall(phoneNumber);
  }
}



//////////////////////////////////////////////////////////
////////////////////   DATE TIME   ///////////////////////
//////////////////////////////////////////////////////////

void GPStimeSync(){
  if(GPSDateTimeSynced==false){
      convertGPStimeToInt();
      time_init();
      LDateTime.setTime(&t);
      LcdExt.screen_set(0x000000);  // flush the screen
  }
}

void convertGPStimeToInt(){
  // get the time from GPS
  utc_date_time = LGPS.get_utc_date_time();
 
  sprintf(bufferDateTime, "%d", utc_date_time[0]);
  GPSyear = atoi(bufferDateTime);
  Serial.println(GPSyear);
  sprintf(bufferDateTime, "%d", utc_date_time[1]);
  GPSmonth = atoi(bufferDateTime);
  Serial.println(GPSmonth);
  sprintf(bufferDateTime, "%d", utc_date_time[2]);
  GPSday = atoi(bufferDateTime);
  Serial.println(GPSday);
  sprintf(bufferDateTime, "%d", utc_date_time[3]);  // get GPS UTC hour data
  GPShour = (atoi(bufferDateTime))+timeZone;  // convert GPS UTC hour data into integer (and add/subtract time zone)
  Serial.println(GPShour);
  sprintf(bufferDateTime, "%d", utc_date_time[4]);
  GPSminute = atoi(bufferDateTime);
  Serial.println(GPSminute);
  sprintf(bufferDateTime, "%d", utc_date_time[5]);
  GPSsec = atoi(bufferDateTime);
  Serial.println(GPSsec);

  GPStimeConverted = true;  // set flag to enter time_init()
}

void time_init()
{
  if(GPStimeConverted==true){
    t.year  = GPSyear+2000;
    t.mon   = GPSmonth;
    t.day   = GPSday;
    t.hour  = GPShour;
    t.min   = GPSminute;
    t.sec   = GPSsec;
    GPSDateTimeSynced = true; // set flag to execute onlt once
  } else {
    t.year  = year;
    t.mon   = month;
    t.day   = day;
    t.hour  = hour;
    t.min   = minute;
    t.sec   = sec;
  }
}




/* ==== PRINT TO PROCESSING ==== */
// creates a string with Acc values to be printed in Processing
// void printToProcessing() {
//    String _analogInPinLabels[3] = {"X", "Y", "Z"};
//    int _analogInPinValues[3] = { data1, data2, data3};
//
//    String str = "";
//    for (int i = 0; i <= 2; i++) {
//        if (str.length() > 0) {
//          str += ",";
//        }
//        str += _analogInPinLabels[i] + "=" + _analogInPinValues[i];
//    }
//    Serial.println(str);
//    delay(2);
// }

/* ==== TEST MODE UI ==== */
//void testModeUI(){}

/* ==== TEST MODE ==== */
//void testMode(){
//  Serial.println("Into Test Mode");
////  Serial.print("Test Mode Status");Serial.println(testModeStatus);
//  if(checkSystem()==1){
//    if(testFlag==0){
//      draw_rect(40,185,160,45,0x00CC00);
//      Lcd.draw_font((xFinal/2)-60,200,"START TEST",0x00CC00,0xffffff, 20);
//      if (Tp.Event_available()) {
//        Tp.Get_event_xy(&EVENT, &X, &Y);
//        if (EVENT>0 && EVENT!=2 && X>=40 && X<=200 && Y>=185 && Y<=230) {
//          draw_rect(40,185,160,45,0x008e00);
//          Lcd.draw_font((xFinal/2)-60,200,"START TEST",0x008e00,0xffffff, 20);
////          delay(500);
//            
//          // if GPS is online:
//          // - create file
//          // - name it with date and time
//          // - add header in the txt file 
////          if(LGPS.check_online()){
////            utc_date_time = LGPS.get_utc_date_time();
//            // check if the GPS is actually ready
//            // by checking if the year displayed is 2016 or later
////            if(utc_date_time[0]>=16){
////              sprintf(fileName, "%d%d%d-%d%d%d.txt", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
////              LFile.Create(fileName);
////              sprintf(fileHeader, "File created: %d-%d-%d %d:%d:%d UTC %d\r\nDelay time: %d\r\n\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5], timeZone,delayTime);
////              LFile.Write(fileName, fileHeader);
////            }          
////          }
//
//            draw_rect(40,185,160,45,0x008e00);
//            Lcd.draw_font((xFinal/2)-75,200,"TEST STARTED",0x008e00,0xffffff, 20);
//            delay(2000);
////            draw_rect(40,185,160,45,0xe5e5ff);
////            Lcd.draw_font((xFinal/2)-65,200,"TEST MODE...",0xe5e5ff,0xffffff);
//
//            testFlag=1;
////            return 1; 
////            int testModeStatus = 1;
//        }
//      }
//    } else if(testFlag==1){
////      draw_rect(40,185,160,45,0xe5e5ff);
////      Lcd.draw_font((xFinal/2)-65,200,"TEST MODE...",0xe5e5ff,0xffffff);
//
//      // create STOP button
//      draw_rect(40,185,160,45,0xff0000);
//      Lcd.draw_font((xFinal/2)-55,200,"STOP TEST",0xff0000,0xffffff, 20);
//      
//      // write only once in the txt that accelerometer
//      // data will be printed
//      if(numAccPrintRuns<maxAccPrintRuns){
//        LFile.Write(fileName, "ACCELEROMETER DATA\r\n\r\n");
//        numAccPrintRuns=1;
//      }
//      
//      // get and print accelerometer data
//      sprintf((char*)writedata, "X:%d\t\t\t Y:%d\t\t\t Z:%d\r\n", accData1, accData2, accData3);
//      LFile.Write(fileName, writedata);
//      LSensorHub.GetAccData(&accData1, &accData2, &accData3);
//      
//      // if STOP button is touched > EXIT
//      if (Tp.Event_available()) {
//        Tp.Get_event_xy(&EVENT, &X, &Y);          
//        if (EVENT>0 && EVENT!=2 && X>=40 && X<=200 && Y>=185 && Y<=230) {
//          draw_rect(40,185,160,45,0x990000);
//          Lcd.draw_font((xFinal/2)-55,200,"STOP TEST",0x990000,0xffffff, 20);
//          delay(2000);
//          testFlag=0;
//          numAccPrintRuns=0;
////          return 0;
////          int testModeStatus = 1;
//        }        
//      }
//    }
//  } else {
//    draw_rect(40,185,160,45,0xe5e5e5);
//    Lcd.draw_font((xFinal/2)-60,200,"START TEST",0xe5e5e5,0xffffff, 20);
////    return 0;
////    int testModeStatus = 0;
//  } 
//}
