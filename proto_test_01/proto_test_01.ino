#include <LDisplay.h>
#include <LSensorHub.h>
#include <LAudio.h>
#include <LGTouch.h>
#include <LGPS.h>
#include <stdio.h>

// initialize variables for audio module
#define    FILE_NAME    "rephone_audio.mp3"
int volume = 6;

// initialize variables for touch event
char EVENT = 0;
int X = 0;
int Y = 0;

int flag = 0;

void setup()
{
    Serial.begin(115200);
//    Serial.print("Sensor Hub test.\r\n");

    LAudio.begin();
    LAudio.setVolume(volume); 
   
    Tp.Init(); 

    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(0xffff00);
    
// OLD STUFF
//    Lcd.draw_font(0, 0, "Sensor Hub Ready", 0xffff00, 0);
//    if(LSensorHub.check_on_line() == false)
//    {
//        Lcd.draw_font(0, 20, "Sensor Hub Off-line", 0xffff00, 0);
//        while(1);
//    }
}

void loop()
{
    long data1 = 0;  // will be used for X axis values
    long data2 = 0;  // will be used for Y axis values
    long data3 = 0;  // will be used for Z axis values
//    unsigned long data4 = 0;  // will be used for Light values
//    long data5 = 0;  // will be used for Temp values


// check if Sensor Hub is connected   
    Lcd.draw_font(20, 20, "Accelerometer Test", 0xffff00, 0);    
    if(LSensorHub.check_on_line() == false)
    {
        Lcd.draw_font(20, 40, "Accelerometer Off-line", 0xffff00, 0);
        while(1);
    }

// get data from sensors    
    LSensorHub.GetAccData(&data1, &data2, &data3);
//    LSensorHub.GetLighData(&data4);
//    LSensorHub.GetTempData(&data5);

    
// creates a string with Acc values to be printed in Processing
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
// --- end of string ---


    unsigned char *utc_date_time = 0;
    char buffer1[21] = {0,};   
    char buffer2[21] = {0,};   
    char buffer3[21] = {0,};   


// check if Free-fall (needs better algorithm)    
    if (data1>=-50 && data1<=50 && data2>=-50 && data2<=50 && data3>=-50 && data3<=50) {      
      flag = 1;
      
      if(LGPS.check_online())
      {
          utc_date_time = LGPS.get_utc_date_time();
          sprintf(buffer1, "on %d-%d-%d  %d:%d:%d\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
          

          sprintf(buffer2, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
          
          
          sprintf(buffer3, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());    
      }
    }

    
    while(flag==1){
      // switch screen color
      Lcd.screen_set(0xff007f);
      Lcd.draw_font(60, 40, "TRIGGERED!", 0xff007f, 0);
      Lcd.draw_font(60, 200, "touch to stop", 0xff007f, 0);
      
      Lcd.draw_font(40, 100, buffer1, 0xff007f, 0);
      Lcd.draw_font(40, 120, buffer2, 0xff007f, 0);
      Lcd.draw_font(40, 140, buffer3, 0xff007f, 0);
      
      // play audio file
      AudioStatus status;
      LAudio.playFile(storageFlash,(char*)FILE_NAME);
      
      delay(1000);

      // check touch event
      if(Tp.Event_available()){
        Tp.Get_event_xy(&EVENT, &X, &Y);
        if(EVENT>0){
          flag = 0;
          Lcd.screen_set(0xffff00);
          break;
        }
      } 
    }
    


    
    
    

// print Acc X value to LCD    
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
// print Acc Y value to LCD
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
// print Acc Z value to LCD
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

// print Light value to LCD    
//    Lcd.draw_font(0, 80, "Light:      ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 80, data4, 0xffff00, 0);
//    Lcd.draw_font(9*16, 80, "Lux", 0xffff00, 0);
//
// print Temp value to LCD
//    Lcd.draw_font(0, 100, "Temp:     ", 0xffff00, 0);
//    Lcd.draw_number(9*10, 100, data5, 0xffff00, 0);
//    Lcd.draw_font(9*16, 100, "'C", 0xffff00, 0);
//    delay(100);
}
