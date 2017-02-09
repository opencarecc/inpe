#include <LDisplay.h>
#include <LSensorHub.h>
#include <LAudio.h>
#include <LGTouch.h>
#include <LGPS.h>
#include <stdio.h>
#include <LCheckSIM.h>
#include <LGSM.h>
#include <LBattery.h>    // Battery library
#include <LCFile.h>

// initialize variables for GSM
char *phoneNumber = "00393402686996";

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

// initialize variables for Light data
//unsigned long data4 = 0;
  
// initialize variables for Temp data
//long data5 = 0;

// initialize variables for GPS UTC time data
unsigned char *utc_date_time = 0;
char bufferDateTime[30] = {0,};
int timeZone = +2;  // add here the amount of hours difference of your time zone compared to GMT

// check to repeat alarm
int flag = 0;

int systemFlag = 0;

int testFlag = 0;

// check to send SMS only once
//int numRuns = 0;
//int maxRuns = 1;

// string to save the SMS SENT or NOT SENT data
char *SMSsentFeedback;

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

void setup()
{
  Serial.begin(115200);
//  Serial.println("Sensor Hub test.\r\n");

  // Initialize Audio module
  LAudio.begin();
  LAudio.setVolume(volume);
  LAudio.playFile(storageFlash, (char*)FILE_NAME);  // play audio file to notify audio system is ready and device is ON

  // initialize touch
  Tp.Init();

  // initialize LCD
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(100);
  Lcd.screen_set(0x000000);
//  vm_graphic_set_font_size(18);  // this is the function in the API to change font size
}

void loop()
{
//  Serial.println("Into LOOP");
  checkSensorHub();
  checkGPS();
  checkSIM();
//  Serial.println("Checked modules");
  
  checkSystem();
//  Serial.println("Checked system");

//  createHeader();
  homeScreen();

//  if(detectFall()==1){
//    Serial.println("Fall detected");
//    flag = 1;
////    doTheCall();
////    doSMS();
//  }
//  
//  while (flag == 1) {
//    doTheAlarm();
//  }
  
  
}


/* ==== UI HEADER ==== */
void createHeader(){
  draw_rect(0, 0, 240, 30, 0xffffff);
  batteryLevel();
}
void displayDate(){
  if(checkGPS()==1){
    utc_date_time = LGPS.get_utc_date_time();
    // print Date and Time
    sprintf(bufferDateTime, "%d-%d-%d", utc_date_time[2], utc_date_time[1], utc_date_time[0]);
    Lcd.draw_font(10, 7, bufferDateTime, 0xffffff, 0x000000);  
  } else {
    Lcd.draw_font(10, 7, "(Date Error)", 0xffffff, 0x000000);
  }
}
void displayTime(){
  if(checkGPS()==1){
    sprintf(bufferDateTime, " %d : %d : %d ", (utc_date_time[3])+timeZone, utc_date_time[4], utc_date_time[5]);
    Lcd.draw_font(65, 65, bufferDateTime, 0, 0xffffff);
  } else {
    Lcd.draw_font(65, 65, "(Time Error)", 0, 0xffffff);
  }
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
  
  // update Display   
//  Lcd.draw_updata();
}

/* ==== CHECK ACCELEROMETER ==== */
/* Check if Accelerometer is working */
boolean checkSensorHub() {
//  Lcd.draw_font(10, 200, "Acc:", 0, 0xffffff);
  switch(LSensorHub.check_on_line()){
    /* if Sensor Hub is not connected */
    case 0:                                           
//      Lcd.draw_font(10, 220, "OFF", 0, 0xff0000);
      sensorHubStatus = "OFF";
      return 0;
      break;
    /* if Sensor Hub is connected */
    case 1:                                           
      LSensorHub.GetAccData(&accData1, &accData2, &accData3);
      /* and if you are receiving values different than zero from the Accelerometer */  
      if (accData1!=0 && accData2!=0 && accData3!=0) {
//        Lcd.draw_font(10, 220, "ON  ", 0, 0xffffff);
        sensorHubStatus = "ON  ";
        return 1;
      }
      break;
  }
}
/* Display Accelerometer State */
void displaySensorHub(){
  Lcd.draw_font(10, 200, "Acc:", 0, 0xffffff);
  if(checkSensorHub()){
    // if SensorHub is connected display sensorHubStatus in white
    Lcd.draw_font(10, 220, sensorHubStatus, 0, 0xffffff);
  } else {
    // if SensorHub is connected display sensorHubStatus in red
    Lcd.draw_font(10, 220, sensorHubStatus, 0, 0xff0000);
  }
}

/* ==== CHECK GPS ==== */
/* Check if GPS is working */
boolean checkGPS() {  
//  Lcd.draw_font((xFinal/2)-30, 200, "GPS:", 0, 0xffffff);
  switch(LGPS.check_online()){
    /* if GPS module is not connected */
    case 0:
//      Lcd.draw_font((xFinal/2)-30, 220, "OFF        ", 0, 0xff0000);
      GPSState = "OFF";
      return 0;
      break;
    /* if GPS module is connected */
    case 1:
//      Lcd.draw_font((xFinal/2)-30, 220, "ON  ", 0, 0xffffff);
      GPSState = "ON  ";
      /* if GPS Status is V(=navigation receiver warning) */
      if (LGPS.get_status()==86) {
//        Lcd.draw_font((xFinal/2)+8, 220, "Sync  ", 0, 0xff0000);
          GPSStatus = "Sync  ";
      }
      /* if GPS Status is A(=Valid position) */
      else if (LGPS.get_status()==65) {
//        Lcd.draw_font((xFinal/2)+8, 220, "Ready", 0, 0xffffff);
        GPSStatus = "Ready";
        return 1;
      } else {
//        Lcd.draw_font((xFinal/2)+8, 220, "Error", 0, 0xff0000);
        GPSStatus = "Error";
      }
      break;
  }

/* Display GPS connection and status Data on LCD for debugging */
//Lcd.draw_font(10, 30, "GPS: ", 0, 0xffffff);
//Lcd.draw_number(60, 30, LGPS.check_online(), 0, 0xffffff);
//Lcd.draw_number(80, 30, LGPS.get_status(), 0, 0xffffff);
}
void displayGPS(){
  Lcd.draw_font((xFinal/2)-30, 200, "GPS:", 0, 0xffffff);
  if (LGPS.check_online()){
    // if GPS is connected display GPSState in white
    Lcd.draw_font((xFinal/2)-30, 220, GPSState, 0, 0xffffff);
    if (LGPS.get_status()==65) {
      // if GPS status is A display GPSStatus in white
      Lcd.draw_font((xFinal/2)+8, 220, GPSStatus, 0, 0xffffff);
    } else {
      // if GPS status is V or error display GPSStatus in red
      Lcd.draw_font((xFinal/2)+8, 220, GPSStatus, 0, 0xff0000);
    }
  } else {
    // if GPS is connected display GPSState in red
    Lcd.draw_font((xFinal/2)-30, 220, GPSState, 0, 0xff0000);
  }
}


/* ==== CHECK SIM ==== */
/* Check if a SIM card is inserted */
boolean checkSIM() {
//  Lcd.draw_font(190, 200, "SIM:", 0, 0xffffff);
  switch(LCheckSIM.isCheck()){
    /* if SIM is not inserted */
    case 0:
//      Lcd.draw_font(190, 220, "Error", 0, 0xff0000);
      SIMStatus = "Error";
      return 0;
      break;
    /* if SIM is inserted */
    case 1:
//      Lcd.draw_font(190, 220, "OK   ", 0, 0xffffff);
      SIMStatus = "OK   ";
      return 1;
      break;
  }
}
void displaySIM(){
  Lcd.draw_font(190, 200, "SIM:", 0, 0xffffff);
  if(LCheckSIM.isCheck()){
    Lcd.draw_font(190, 220, SIMStatus, 0, 0xffffff);
  } else {
    Lcd.draw_font(190, 220, SIMStatus, 0, 0xff0000);
  }
}

/* ==== CHECK SYSTEM READY ==== */
// Check if System is ready
boolean checkSystem(){
  if(checkSensorHub()==1 && checkGPS()==1 && checkSIM()==1){
    systemCheck = " Ready ";
//    Lcd.draw_font(55, 130, "System Ready", 0, 0xffffff);
//    Serial.println("Ready");
    return 1;
  } else {
    systemCheck = " Error ";
//    Lcd.draw_font(55, 130, "System Error ", 0x000000, 0xffffff);
//    Serial.println("System Error");
    return 0;
  }
}

/* ==== HOME SCREEN ==== */
void homeScreen(){
  Serial.println("Into Home Screen");
  
  
  
  
  
//  Lcd.screen_set(0x000000);
// flush content by creating a new background
  draw_rect(0,30,240,210,0x000000);
  
  while(systemFlag==0 && flag==0){
    Serial.println("Into Home Screen While");
    createHeader();
    displayDate();
    displayTime(); 
    
    Lcd.draw_font(85, 105, "In Pe' is", 0, 0xffffff);
    if(checkSystem()==1){
      // font color green
      draw_rect(50,130,150,40,0x000000);
      Lcd.draw_font(89, 130, systemCheck, 0, 0x00ff00);
    } else {
      // font color red
      draw_rect(50,130,150,40,0x000000);
      Lcd.draw_font(89, 130, systemCheck, 0, 0xff0000);
      Lcd.draw_font(50, 150, "Check System >", 0, 0xff0000);
    }
    
    if (Tp.Event_available()) {
      Tp.Get_event_xy(&EVENT, &X, &Y);
      goToSystemCheckScreen();    
        
    }
    testMode();
    
    if(detectFall()==1){
        Serial.println("Fall detected");
        flag = 1;
        //doTheCall();
        doSMS();
        doTheAlarm();
      }
    

//    displaySensorHub();
//    displayGPS();
//    displaySIM();    

//LSensorHub.GetAccData(&accData1, &accData2, &accData3);
//Serial.print("X ");Serial.println(accData1);
//Serial.print("Y ");Serial.println(accData2);
//Serial.print("Z ");Serial.println(accData3);

    
    
//    while (flag == 1) {
//      Serial.println("Into Alarm while");
//      doTheAlarm();
//    }

//while(flag == 1) {
//      Serial.println("Into Alarm while");
//      doTheAlarm();
//    }

  }   
}

void goToSystemCheckScreen(){
//  if (Tp.Event_available()) {
//    Serial.println("touched");
//    Tp.Get_event_xy(&EVENT, &X, &Y);
//    Serial.println(X);
//    Serial.println(Y);
    if (EVENT>0 && X>=40 && X<=210 && Y>=140 && Y<=180) {
//      Serial.println("systemCheckScreen");
      systemFlag = 1;
      systemCheckScreen();     
    }
//  }
}

/* ==== FALL DETECTION ==== */
boolean detectFall() {
  // get data from sensors
  LSensorHub.GetAccData(&accData1, &accData2, &accData3);
  // check if Free-fall (needs better algorithm)
  if (accData1 >= -50 && accData1 <= 50 && accData2 >= -50 && accData2 <= 50 && accData3 >= -50 && accData3 <= 50) {
//    flag = 1;
    sprintf(detected, "DETECTED!\r\non %d-%d-%d %d:%d:%d\r\nat LAT %c:%f LONG %c:%f\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4], utc_date_time[5], LGPS.get_ns(), LGPS.get_latitude(), LGPS.get_ew(), LGPS.get_longitude());
    LFile.Write(fileName, detected);
    return 1;
  } else {
    return 0;
  }
  
  
    
//    if (EVENT>0 && X>=0 && X<=240 && Y>=0 && Y<=40) {   
//      sprintf(detected, "DETECTED!\r\non %d-%d-%d %d:%d:%d\r\nat LAT %c:%f LONG %c:%f\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4], utc_date_time[5], LGPS.get_ns(), LGPS.get_latitude(), LGPS.get_ew(), LGPS.get_longitude());
//      LFile.Write(fileName, detected);
//      return 1; 
//    } else {
//      return 0;
//    }
}


/* ==== START A CALL TO SELECTED NUMBER ==== */
void doTheCall() {
//Serial.println("do the call");
  // check if SIM is ready
  if (LCheckSIM.isCheck() == 1) {
    // start call
    LVoiceCall.voiceCall(phoneNumber);
  }
}

/* ==== SEND SMS TO SELECTED NUMBER ==== */
void doSMS() {
//  if(numRuns<maxRuns){
    char SMScontent[159] = {0,};
    sprintf(SMScontent, "HELP! I fell here http://maps.google.com/maps?q=%f,%f", LGPS.get_latitude(), LGPS.get_longitude());
  
    Serial.println("Sending SMS:");
//    while(!LSMS.ready()){}
    if(LSMS.ready()){
      Serial.println("SMS ready is ok");
  
      LSMS.beginSMS(phoneNumber);
      LSMS.print(SMScontent);
      
      if(LSMS.endSMS()){
        Serial.println("SMS is sent");
        SMSsentFeedback = "  SMS sent to:";
      } else {
        Serial.println("SMS send fail");
        SMSsentFeedback = "SMS failed to:";
      }
    } else {
      Serial.println("SMS not ready");
      SMSsentFeedback = "SMS Error to:";
    }
       
//    numRuns++;
//  }
}


/* ==== START ALARM ==== */
void doTheAlarm() {

  // switch screen color
//  Lcd.screen_set(0xff007f);
draw_rect(0,0,240,240,0xff007f);
  
  while(flag==1){
  Serial.println("Into Alarm While");
  // initialize var for GPS data
//  unsigned char *utc_date_time = 0;
//  char buffer1[21] = {0,};
//  char buffer2[21] = {0,};
//  char buffer3[21] = {0,};

  // check if GPS is ready
//  if (LGPS.check_online())
//  {
//    utc_date_time = LGPS.get_utc_date_time();
//    // get date
//    sprintf(buffer1, "on %d-%d-%d  %d:%d:%d\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4], utc_date_time[5]);
//    // get GPS latitutde
//    sprintf(buffer2, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
//    // get GPS longitude
//    sprintf(buffer3, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());
//  }

  

  // display triggered text
  Lcd.draw_font(40, 40, "FALL DETECTED!", 0xff007f, 0);
//  Lcd.draw_font(80, 80, "Calling", 0xff007f, 0);
//  Lcd.draw_font(50, 80, SMSsentFeedback, 0xff007f, 0);
  Lcd.draw_font(50, 80, "  SMS sent to:", 0xff007f, 0);
  Lcd.draw_font(40, 100, phoneNumber, 0xff007f, 0);
  
  // if not into STATUS MODE
  if (testFlag==0){
    Lcd.draw_font(60, 200, "touch to stop", 0xff007f, 0);
  }
  if (testFlag==1){
    Lcd.draw_font(60, 140, "Confirm fall?", 0xff007f, 0);
    draw_rect((xFinal/2)-80,170,60,60,0x000000);
    Lcd.draw_font((xFinal/2)-70, 190, "YES", 0x000000, 0xff007f);
    draw_rect((xFinal/2)+20,170,60,60,0x000000);
    Lcd.draw_font((xFinal/2)+35, 190, "NO", 0x000000, 0xff007f);
  }
  delay(1000);
  
  // check touch event and exit triggered state if LCD is touched
  if (Tp.Event_available()) {
    if (testFlag==0){
      Tp.Get_event_xy(&EVENT, &X, &Y);
      if (EVENT > 0) {
        
//        Serial.println("Touch event, flag = 0");
  //      Serial.println(EVENT);
        flag = 0;
        homeScreen();
        
  //      numRuns=0;
        // close call
        LVoiceCall.hangCall();
        
        // set screen to prev background color
//        Lcd.screen_set(0x000000);
  
//        EVENT = 0;
//        return;
      }
    } else
    if (testFlag==1){
      Tp.Get_event_xy(&EVENT, &X, &Y);
      // if press Confirm YES
      if (EVENT > 0 && X>=(xFinal/2)-80 && X<=(xFinal/2)-20 && Y>=170 && Y<=230) {
        draw_rect((xFinal/2)-80,170,60,60,0xffffff);
        Lcd.draw_font((xFinal/2)-70, 190, "YES", 0xffffff, 0xff007f);
        LFile.Write(fileName, "[CONFIRMED]");
        testFlag=0;
        flag = 0;
        homeScreen();
      } else
      if (EVENT > 0 && X>=(xFinal/2)+20 && X<=(xFinal/2)+80 && Y>=170 && Y<=230) {
        draw_rect((xFinal/2)+20,170,60,60,0xffffff);
        Lcd.draw_font((xFinal/2)+35, 190, "NO", 0xffffff, 0xff007f);
        LFile.Write(fileName, "[NOT CONFIRMED]");
        testFlag=0;
        flag = 0;
        homeScreen();
      }
    }
  }

  // display GPS data
//  Lcd.draw_font(40, 100, buffer1, 0xff007f, 0);
//  Lcd.draw_font(40, 120, buffer2, 0xff007f, 0);
//  Lcd.draw_font(40, 140, buffer3, 0xff007f, 0);

  // play audio file
//  Serial.println("Play the audio");
//  AudioStatus status;
//  LAudio.playFile(storageFlash, (char*)FILE_NAME);
//
//  delay(1000);
  }
    
  
/* IDLE CALL - not working because of voice mail */
//  if (LVoiceCall.getVoiceCallStatus() == IDLE_CALL) {
//    Serial.println("Call in idle");
//    // switch screen color
//    Lcd.screen_set(0xff007f);
//  
//    // display triggered text
//    Lcd.draw_font(40, 40, "FALL DETECTED!", 0xff007f, 0);
//    Lcd.draw_font(60, 80, "No answer from", 0xff007f, 0);
//    Lcd.draw_font(40, 100, phoneNumber, 0xff007f, 0);
//    
//    flag = 0;
//    return;
//  }
  
//  delay(50);
}



/* ==== SYSTEM CHECK SCREEN ==== */
void systemCheckScreen() {  
  
//  Lcd.screen_set(0x000000);
// flush content by creating a new background
draw_rect(0,30,240,210,0x000000);

  while(systemFlag == 1){
    Serial.println("Into System Check Screen While");
    createHeader();
    displayDate();
    displaySensorHubValuesLCD();
    displayGPSDataLCD();
    displaySIMLCD();
    draw_rect((xFinal)-87, 205, 87, 40, 0xffffff);
    Lcd.draw_font((xFinal)-78, 215, "< Back", 0xffffff, 0x000000);
    
    // if a touch event is detected
    if (Tp.Event_available()) {
      Tp.Get_event_xy(&EVENT, &X, &Y);
      if (EVENT > 0 && X>=(xFinal)-87 &&  Y >= 220) {
        systemFlag = 0;
        homeScreen();     
      }
    }
  }
}

/* ==== DISPLAY SIM DATA TO LCD ==== */
void displaySIMLCD(){
  Lcd.draw_font(10, 180, "SIM Status:", 0, 0xffffff);
  if(LCheckSIM.isCheck()){
    Lcd.draw_font(120, 180, SIMStatus, 0, 0xffffff);
  } else {
    Lcd.draw_font(120, 180, SIMStatus, 0, 0xff0000);
  }
}

/* ==== DISPLAY GPS DATA TO LCD ==== */
void displayGPSDataLCD() {
  Lcd.draw_font(10, 110, "GPS:", 0, 0xffffff);
  if (LGPS.check_online()){
    // if GPS is connected display GPSState in white
    Lcd.draw_font(60, 110, GPSState, 0, 0xffffff);
  } else {
    Lcd.draw_font(60, 110, GPSState, 0, 0xff0000);
  }  
  char GPSDataBuffer[50] = {0,};
  sprintf(GPSDataBuffer, "Status: %c\r\n", LGPS.get_status());
  Lcd.draw_font(120, 110, GPSDataBuffer, 0, 0xffffff);
  sprintf(GPSDataBuffer, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
  Lcd.draw_font(10, 130, GPSDataBuffer, 0, 0xffffff);
  sprintf(GPSDataBuffer, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());
  Lcd.draw_font(10, 150, GPSDataBuffer, 0, 0xffffff);
}


/* ==== DISPLAY SENSOR HUB VALUES TO LCD ==== */
 void displaySensorHubValuesLCD() {
    // get data from sensors
    LSensorHub.GetAccData(&accData1, &accData2, &accData3);
    Lcd.draw_font(10, 40, "Accelerometer:", 0, 0xffffff);
    if(checkSensorHub()){
      // if SensorHub is connected display sensorHubStatus in white
      Lcd.draw_font(150, 40, sensorHubStatus, 0, 0xffffff);
    } else {
      // if SensorHub is connected display sensorHubStatus in red
      Lcd.draw_font(150, 40, sensorHubStatus, 0, 0xff0000);
    }
/* print Acc X value to LCD */
    Lcd.draw_font(10, 60, "X:", 0, 0xffffff);
    if(accData1 > 0){
//        Lcd.draw_font(9*9, 20, "+", 0xffff00, 0);
//        Lcd.draw_number(9*10, 20, data1, 0xffff00, 0);
        Lcd.draw_font(10, 80, "+", 0, 0xffffff);
        Lcd.draw_number(20, 80, accData1, 0, 0xffffff);
    } else {
        accData1 ^= 0xffffffff;
        accData1 += 1;
        Lcd.draw_font(10, 80, "-", 0, 0xffffff);
        Lcd.draw_number(20, 80, accData1, 0, 0xffffff);
    }
//    Lcd.draw_font(9*16, 20, "mg", 0xffff00, 0);

/* print Acc Y value to LCD */
    Lcd.draw_font((xFinal/2)-20, 60, "Y:", 0, 0xffffff);
    if(accData2 > 0){
        Lcd.draw_font((xFinal/2)-20, 80, "+", 0, 0xffffff);
        Lcd.draw_number((xFinal/2)-10, 80, accData2, 0, 0xffffff);
    } else {
        accData2 ^= 0xffffffff;
        accData2 += 1;
        Lcd.draw_font((xFinal/2)-20, 80, "-", 0, 0xffffff);
        Lcd.draw_number((xFinal/2)-10, 80, accData2, 0, 0xffffff);
    }
//    Lcd.draw_font(9*16, 40, "mg", 0xffff00, 0);

/* print Acc Z value to LCD */
    Lcd.draw_font((xFinal)-50, 60, "Z:", 0, 0xffffff);
    if(accData3 > 0){
        Lcd.draw_font((xFinal)-50, 80, "+", 0, 0xffffff);
        Lcd.draw_number((xFinal)-40, 80, accData3, 0, 0xffffff);
    } else {
        accData3 ^= 0xffffffff;
        accData3 += 1;
        Lcd.draw_font((xFinal)-50, 80, "-", 0, 0xffffff);
        Lcd.draw_number((xFinal)-40, 80, accData3, 0, 0xffffff);
    }
//    Lcd.draw_font(9*16, 60, "mg", 0xffff00, 0);

///* print Light value to LCD */
//    Lcd.draw_font(0, 80, "Light:      ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 80, data4, 0xffff00, 0);
//    Lcd.draw_font(9*16, 80, "Lux", 0xffff00, 0);
//
///* print Temp value to LCD */
//    Lcd.draw_font(0, 100, "Temp:     ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 100, data5, 0xffff00, 0);
//    Lcd.draw_font(9*16, 100, "'C", 0xffff00, 0);
//  delay(100);
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
void testModeUI(){}

/* ==== TEST MODE ==== */
void testMode(){
  Serial.println("Into Test Mode");
//  Serial.print("Test Mode Status");Serial.println(testModeStatus);
  if(checkSystem()==1){
    if(testFlag==0){
      draw_rect(40,185,160,45,0x00CC00);
      Lcd.draw_font((xFinal/2)-60,200,"START TEST",0x00CC00,0xffffff);
      if (Tp.Event_available()) {
        Tp.Get_event_xy(&EVENT, &X, &Y);
        if (EVENT>0 && X>=40 && X<=200 && Y>=185 && Y<=230) {
          draw_rect(40,185,160,45,0x008e00);
          Lcd.draw_font((xFinal/2)-60,200,"START TEST",0x008e00,0xffffff);
//          delay(500);
            
          // if GPS is online:
          // - create file
          // - name it with date and time
          // - add header in the txt file 
//          if(LGPS.check_online()){
//            utc_date_time = LGPS.get_utc_date_time();
            // check if the GPS is actually ready
            // by checking if the year displayed is 2016 or later
//            if(utc_date_time[0]>=16){
              sprintf(fileName, "%d%d%d-%d%d%d.txt", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
              LFile.Create(fileName);
              sprintf(fileHeader, "File created: %d-%d-%d %d:%d:%d UTC %d\r\nDelay time: %d\r\n\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5], timeZone,delayTime);
              LFile.Write(fileName, fileHeader);
//            }          
//          }

            draw_rect(40,185,160,45,0x008e00);
            Lcd.draw_font((xFinal/2)-75,200,"TEST STARTED",0x008e00,0xffffff);
            delay(2000);
//            draw_rect(40,185,160,45,0xe5e5ff);
//            Lcd.draw_font((xFinal/2)-65,200,"TEST MODE...",0xe5e5ff,0xffffff);

            testFlag=1;
//            return 1; 
//            int testModeStatus = 1;
        }
      }
    } else if(testFlag==1){
//      draw_rect(40,185,160,45,0xe5e5ff);
//      Lcd.draw_font((xFinal/2)-65,200,"TEST MODE...",0xe5e5ff,0xffffff);

      // create STOP button
      draw_rect(40,185,160,45,0xff0000);
      Lcd.draw_font((xFinal/2)-55,200,"STOP TEST",0xff0000,0xffffff);
      
      // write only once in the txt that accelerometer
      // data will be printed
      if(numAccPrintRuns<maxAccPrintRuns){
        LFile.Write(fileName, "ACCELEROMETER DATA\r\n\r\n");
        numAccPrintRuns=1;
      }
      
      // get and print accelerometer data
      sprintf((char*)writedata, "X:%d\t\t\t Y:%d\t\t\t Z:%d\r\n", accData1, accData2, accData3);
      LFile.Write(fileName, writedata);
      LSensorHub.GetAccData(&accData1, &accData2, &accData3);
      
      // if STOP button is touched > EXIT
      if (Tp.Event_available()) {
        Tp.Get_event_xy(&EVENT, &X, &Y);          
        if (EVENT>0 && X>=40 && X<=200 && Y>=185 && Y<=230) {
          draw_rect(40,185,160,45,0x990000);
          Lcd.draw_font((xFinal/2)-55,200,"STOP TEST",0x990000,0xffffff);
          delay(2000);
          testFlag=0;
          numAccPrintRuns=0;
//          return 0;
//          int testModeStatus = 1;
        }        
      }
    }
  } else {
    draw_rect(40,185,160,45,0xe5e5e5);
    Lcd.draw_font((xFinal/2)-60,200,"START TEST",0xe5e5e5,0xffffff);
//    return 0;
//    int testModeStatus = 0;
  } 
}
