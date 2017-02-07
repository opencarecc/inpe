
#include <LCheckSIM.h>
#include <LGSM.h>
#include "LDisplay.h"

char * text = "Hi,sms send test ok.";
char num[20] = "003402686996";
char buf_contex[200];

void setup() 
{    
//    Serial.begin(115200);
//    Serial.print("\r\n");  
//    Serial.println("SMS test");

    // initialize LCD display
    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(0x000000);
    
    while(!LSMS.ready())
    {
        delay(1000);
    }
    Lcd.draw_font(10,10,"SMS ready!", 0x000000, 0xffffff, 20);
    
//    LSMS.beginSMS(num);
//    LSMS.print(text);
//    
//    if(LSMS.endSMS())
//    {
//        Serial.println("SMS is sent");
//    }
//    else
//    {
//        Serial.println("SMS send fail");
//    } 
    
    Lcd.draw_updata();
}

void loop() 
{   
    sms_test();
//    Serial.println("loop.");
//    delay(1000);
    Lcd.draw_updata();
}

void sms_test()
{
    if(!LSMS.available())
    {
//         Serial.println("No New SMS");
         Lcd.draw_font(10,30,"SMS: No New    ", 0x000000, 0xffffff, 20);
         return;   
    }

//    Serial.println("Get new sms: ");
    Lcd.draw_font(10,30,"SMS: Received", 0x000000, 0xffffff, 20);

//    LSMS.remoteNumber(num, 20);         // display Number part
    //LSMS.remoteNumber(num, 20);         // display Number part
//    Serial.print("Number:");
//    Serial.println(num);
    
    LSMS.remoteContent(buf_contex, 50);
    //LSMS.remoteNumber(num, 20);         // display Number part
//    Serial.print("Content:");           // display Content part
//    Serial.println(buf_contex);

    if(buf_contex[0]=='T'&&buf_contex[1]=='e')
    {
      Lcd.draw_font(10,70,"T", 0x000000, 0xffffff, 20);
      Lcd.draw_updata();
      if(buf_contex[2]=='0'){
        Lcd.screen_set(0xffffff);
        Lcd.draw_font(40,70,"0", 0x000000, 0xffffff, 20);
      }
      if(buf_contex[2]=='1'){
        Lcd.screen_set(0xffff00);
        Lcd.draw_font(40,70,"1", 0x000000, 0xffffff, 20);
      }
//        while(!LSMS.ready())
//        {
//            delay(1000);
//        }
//       
//        Serial.println("SIM ready for work!");
//        LSMS.beginSMS(num);
//        LSMS.print(text);
//        
//        if(LSMS.endSMS())
//        {
//            Serial.println("SMS is sent");
//        }
//        else
//        {
//            Serial.println("SMS send fail");
//        } 
        LSMS.flush();  
    }
}
