#include <LDisplay.h>
#include <LSensorHub.h>
#include <LAudio.h>
#include <LGTouch.h>
#include <LGPS.h>
#include <stdio.h>
#include <LCheckSIM.h>
#include <LGSM.h>
#include <LBattery.h>    // Battery library

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
long data1 = 0;  // will be used for X axis values
long data2 = 0;  // will be used for Y axis values
long data3 = 0;  // will be used for Z axis values

// initialize variables for Light data
//unsigned long data4 = 0;
  
// initialize variables for Temp data
//long data5 = 0;

// initialize variables for GPS UTC time data
unsigned char *utc_date_time = 0;
char bufferDateTime[30] = {0,};

// check to repeat alarm
int flag = 0;

// check to send SMS only once
//int numRuns = 0;
//int maxRuns = 1;

// string to save the SMS SENT or NOT SENT data
char *SMSsentFeedback;

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
  Lcd.back_light_level(50);
  Lcd.screen_set(0x000000);
//  vm_graphic_set_font_size(18);
}

void loop()
{
  checkSensorHub();
  checkGPS();
  checkSIM();
  
  // Check if System is ready
  if(checkSensorHub()==1 && checkGPS()==1 && checkSIM()==1){
    Lcd.draw_font(55, 130, "System Ready", 0, 0xffffff);
//    Serial.println("Ready");
  } else {
    Lcd.draw_font(55, 130, "System Error ", 0x000000, 0xffffff);
  }
  
  createHeader();
  displayDate(); 
  displayTime(); 
  Lcd.draw_font(80, 105, "In Pe' is", 0, 0xffffff);
  
  if(detectFall()==1){
    Serial.println("Fall detected");
    flag = 1;
//    doTheCall();
//    doSMS();
  }
  
  while (flag == 1) {
    doTheAlarm();
  }
  
  
}


/* ==== HEADER ==== */
void createHeader(){
  draw_rect(0, 0, 240, 30, 0xffffff);
  batteryLevel();
}
void displayDate(){
  utc_date_time = LGPS.get_utc_date_time();
  // print Date and Time
  sprintf(bufferDateTime, "%d-%d-%d", utc_date_time[2], utc_date_time[1], utc_date_time[0]);
  Lcd.draw_font(10, 7, bufferDateTime, 0xffffff, 0x000000);   
}
void displayTime(){
  sprintf(bufferDateTime, "%d : %d : %d ", utc_date_time[3], utc_date_time[4], utc_date_time[5]);
  Lcd.draw_font(70, 65, bufferDateTime, 0, 0xffffff);
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
  Lcd.draw_font(10, 200, "Acc:", 0, 0xffffff);
  switch(LSensorHub.check_on_line()){
    /* if Sensor Hub is not connected */
    case 0:                                           
      Lcd.draw_font(10, 220, "OFF", 0, 0xffffff);
      return 0;
      break;
    /* if Sensor Hub is connected */
    case 1:                                           
      LSensorHub.GetAccData(&data1, &data2, &data3);
      /* and if you are receiving values different than zero from the Accelerometer */  
      if (data1!=0 && data2!=0 && data3!=0) {
        Lcd.draw_font(10, 220, "ON  ", 0, 0xffffff);
      }
      return 1;
      break;
  }
}

/* ==== CHECK GPS ==== */
/* Check if GPS is working */
boolean checkGPS() {  
  Lcd.draw_font((xFinal/2)-30, 200, "GPS:", 0, 0xffffff);
  switch(LGPS.check_online()){
    /* if GPS module is not connected */
    case 0:
      Lcd.draw_font((xFinal/2)-30, 220, "OFF        ", 0, 0xffffff);
      return 0;
      break;
    /* if GPS module is connected */
    case 1:
      Lcd.draw_font((xFinal/2)-30, 220, "ON  ", 0, 0xffffff);
      /* if GPS Status is V(=navigation receiver warning) */
      if (LGPS.get_status()==86) {
        Lcd.draw_font((xFinal/2)+8, 220, "Sync  ", 0, 0xffffff);
      }
      /* if GPS Status is A(=Valid position) */
      else if (LGPS.get_status()==65) {
        Lcd.draw_font((xFinal/2)+8, 220, "Ready", 0, 0xffffff);
        return 1;
      } else {
        Lcd.draw_font((xFinal/2)+8, 220, "Error", 0, 0xffffff);
      }
      break;
  }

/* Display GPS connection and status Data on LCD for debugging */
//Lcd.draw_font(10, 30, "GPS: ", 0, 0xffffff);
//Lcd.draw_number(60, 30, LGPS.check_online(), 0, 0xffffff);
//Lcd.draw_number(80, 30, LGPS.get_status(), 0, 0xffffff);
}


/* ==== CHECK SIM ==== */
/* Check if a SIM card is inserted */
boolean checkSIM() {
  Lcd.draw_font(190, 200, "SIM:", 0, 0xffffff);
  switch(LCheckSIM.isCheck()){
    /* if SIM is not inserted */
    case 0:
      Lcd.draw_font(190, 220, "Error", 0, 0xffffff);
      return 0;
      break;
    /* if SIM is inserted */
    case 1:
      Lcd.draw_font(190, 220, "OK   ", 0, 0xffffff);
      return 1;
      break;
  }
}

/* ==== FALL DETECTION ==== */
boolean detectFall() {
  // get data from sensors
  LSensorHub.GetAccData(&data1, &data2, &data3);
  // check if Free-fall (needs better algorithm)
  if (data1 >= -50 && data1 <= 50 && data2 >= -50 && data2 <= 50 && data3 >= -50 && data3 <= 50) {
//    flag = 1;
    return 1;
  } else {
    return 0;
  }
}



void doTheCall() {
//Serial.println("do the call");
  // check if SIM is ready
  if (LCheckSIM.isCheck() == 1) {
    // start call
    LVoiceCall.voiceCall(phoneNumber);
  }
}

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



void doTheAlarm() {
  Serial.println("do the alarm");
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

  // switch screen color
  Lcd.screen_set(0xff007f);

  // display triggered text
  Lcd.draw_font(40, 40, "FALL DETECTED!", 0xff007f, 0);
//  Lcd.draw_font(80, 80, "Calling", 0xff007f, 0);
  Lcd.draw_font(50, 80, SMSsentFeedback, 0xff007f, 0);
  Lcd.draw_font(40, 100, phoneNumber, 0xff007f, 0);
  
  
  Lcd.draw_font(60, 200, "touch to stop", 0xff007f, 0);

  // display GPS data
//  Lcd.draw_font(40, 100, buffer1, 0xff007f, 0);
//  Lcd.draw_font(40, 120, buffer2, 0xff007f, 0);
//  Lcd.draw_font(40, 140, buffer3, 0xff007f, 0);

  // play audio file
  Serial.println("Play the audio");
  AudioStatus status;
  LAudio.playFile(storageFlash, (char*)FILE_NAME);

  delay(1000);

  // check touch event and exit triggered state if LCD is touched
  if (Tp.Event_available()) {
    Tp.Get_event_xy(&EVENT, &X, &Y);
    if (EVENT > 0) {
      
      Serial.println("Touch event, flag = 0");
//      Serial.println(EVENT);
      flag = 0;
//      numRuns=0;
      // close call
      LVoiceCall.hangCall();
      
      // set screen to prev background color
      Lcd.screen_set(0x000000);

      EVENT = 0;
      return;
    }
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
  
  delay(50);
}



/* ==== DISPLAY GPS DATA TO LCD ==== */
//void displayGPSDataLCD() {
//  char GPSDataBuffer[50] = {0,};
//  sprintf(GPSDataBuffer, "Status %c\r\n", LGPS.get_status());
//  Lcd.draw_font(10, 100, GPSDataBuffer, 0, 0xffffff);
//  sprintf(GPSDataBuffer, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
//  Lcd.draw_font(10, 120, GPSDataBuffer, 0, 0xffffff);
//  sprintf(GPSDataBuffer, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());
//  Lcd.draw_font(10, 140, GPSDataBuffer, 0, 0xffffff);
//}


/* ==== DISPLAY SENSOR HUB VALUES TO LCD ==== */
// void displaySensorHubValuesLCD() {
///* print Acc X value to LCD */
//    Lcd.draw_font(0, 20, "Acc X:        ", 0xffff00, 0);
//    if(data1 > 0)
//    {
//        Lcd.draw_font(9*9, 20, "+", 0xffff00, 0);
//  Lcd.draw_number(9*10, 20, data1, 0xffff00, 0);
//    }
//    else
//    {
//        data1 ^= 0xffffffff;
//        data1 += 1;
//        Lcd.draw_font(9*9, 20, "-", 0xffff00, 0);
//  Lcd.draw_number(9*10, 20, data1, 0xffff00, 0);
//    }
//    Lcd.draw_font(9*16, 20, "mg", 0xffff00, 0);
//
///* print Acc Y value to LCD */
//    Lcd.draw_font(0, 40, "Acc Y:        ", 0xffff00, 0);
//    if(data2 > 0)
//    {
//        Lcd.draw_font(9*9, 40, "+", 0xffff00, 0);
//  Lcd.draw_number(9*10, 40, data2, 0xffff00, 0);
//    }
//    else
//    {
//        data2 ^= 0xffffffff;
//        data2 += 1;
//        Lcd.draw_font(9*9, 40, "-", 0xffff00, 0);
//  Lcd.draw_number(9*10, 40, data2, 0xffff00, 0);
//    }
//    Lcd.draw_font(9*16, 40, "mg", 0xffff00, 0);
//
///* print Acc Z value to LCD */
//    Lcd.draw_font(0, 60, "Acc Z:        ", 0xffff00, 0);
//    if(data3 > 0)
//    {
//        Lcd.draw_font(9*9, 60, "+", 0xffff00, 0);
//  Lcd.draw_number(9*10, 60, data3, 0xffff00, 0);
//    }
//    else
//    {
//        data3 ^= 0xffffffff;
//        data3 += 1;
//        Lcd.draw_font(9*9, 60, "-", 0xffff00, 0);
//  Lcd.draw_number(9*10, 60, data3, 0xffff00, 0);
//    }
//    Lcd.draw_font(9*16, 60, "mg", 0xffff00, 0);
//
///* print Light value to LCD */
//    Lcd.draw_font(0, 80, "Light:      ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 80, data4, 0xffff00, 0);
//    Lcd.draw_font(9*16, 80, "Lux", 0xffff00, 0);
//
///* print Temp value to LCD */
//    Lcd.draw_font(0, 100, "Temp:     ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 100, data5, 0xffff00, 0);
//    Lcd.draw_font(9*16, 100, "'C", 0xffff00, 0);
//    delay(100);
// }


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

