
#include <LDisplay.h>
#include <LSensorHub.h>
#include <LAudio.h>

// add your music file name here
#define    FILE_NAME    "rephone_audio.mp3"

int volume = 6;

void setup()
{
    Serial.begin(115200);
//    Serial.print("Sensor Hub test.\r\n");

    LAudio.begin();
    LAudio.setVolume(volume);  

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
    
// check if Sensor Hub is connected    
    Lcd.draw_font(0, 0, "Accelerometer Ready", 0xffff00, 0);
    if(LSensorHub.check_on_line() == false)
    {
        Lcd.draw_font(0, 20, "Accelerometer Off-line", 0xffff00, 0);
        while(1);
    }

// check if Free-fall (needs better algorithm)    
    if (data1>=-50 && data1<=50 && data2>=-50 && data2<=50 && data3>=-50 && data3<=50) {
      // switch screen color
      Lcd.screen_set(0xff007f);
      Lcd.draw_font(60, 90, "TRIGGERED!", 0xff007f, 0); 
      
      // play audio file
      AudioStatus status;
      LAudio.playFile(storageFlash,(char*)FILE_NAME); 
      
      delay(5000);
      Lcd.screen_set(0xffff00);
    }
    
    

// print Acc X value to LCD    
//    Lcd.draw_font(0, 20, "Acc X:        ", 0xffff00, 0);
//    if(data1 > 0)
//    {
//        Lcd.draw_font(9*9, 20, "+", 0xffff00, 0);
//	Lcd.draw_number(9*10, 20, data1, 0xffff00, 0);
//    }
//    else
//    {
//        data1 ^= 0xffffffff;
//        data1 += 1;
//        Lcd.draw_font(9*9, 20, "-", 0xffff00, 0);
//	Lcd.draw_number(9*10, 20, data1, 0xffff00, 0);
//    }
//    Lcd.draw_font(9*16, 20, "mg", 0xffff00, 0);
//
// print Acc Y value to LCD
//    Lcd.draw_font(0, 40, "Acc Y:        ", 0xffff00, 0);
//    if(data2 > 0)
//    {
//        Lcd.draw_font(9*9, 40, "+", 0xffff00, 0);
//	Lcd.draw_number(9*10, 40, data2, 0xffff00, 0);
//    }
//    else
//    {
//        data2 ^= 0xffffffff;
//        data2 += 1;
//        Lcd.draw_font(9*9, 40, "-", 0xffff00, 0);
//	Lcd.draw_number(9*10, 40, data2, 0xffff00, 0);
//    }
//    Lcd.draw_font(9*16, 40, "mg", 0xffff00, 0);
//
// print Acc Z value to LCD
//    Lcd.draw_font(0, 60, "Acc Z:        ", 0xffff00, 0);
//    if(data3 > 0)
//    {
//        Lcd.draw_font(9*9, 60, "+", 0xffff00, 0);
//	Lcd.draw_number(9*10, 60, data3, 0xffff00, 0);
//    }
//    else
//    {
//        data3 ^= 0xffffffff;
//        data3 += 1;
//        Lcd.draw_font(9*9, 60, "-", 0xffff00, 0);
//	Lcd.draw_number(9*10, 60, data3, 0xffff00, 0);
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
