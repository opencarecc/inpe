/*
  Battery Test for Rephone Kit
  
  Hello, this sketch will show the status of the battery
  on the Serial Monitor or on the Xadow 1.54" Touchscreen v1.0 module
  
  Alessandro Contini for http://opencare.cc/
  August 2016
*/

#include <LBattery.h>    // Battery library
#include <LDisplay.h>    // LCD library

// set min and max coordinates for easier positioning on LCD screen
int xInit = 0;
int yInit = 0;
int xFinal = 240;
int yFinal = 240;

// create a function to draw a rectangle using inputs:
//    - X and Y coordinates for positioning of the first pixel
//    - width and height for rectangle size
void draw_rect(int x, int y, int width, int height, uint32_t color){
  Lcd.draw_fill_rectangle(x,y,(x+width),(y+height),color);
}

void setup() 
{
    // start communication with Serial Monitor
    Serial.begin(115200);
    Serial.print("Battery test.\r\n");
    
    // initialize LCD display
    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(0xffff00);
}

void loop() 
{   
    // write text on screen
    Lcd.draw_font((xFinal/4),(yFinal/4),"Battery level is\r\n", 0xffff00, 0);
    
    // draw battery icon
    draw_rect((xFinal/2)-30,(yFinal/2)-15,60,30,0x000000);
    draw_rect((xFinal/2)+30,(yFinal/2)-10,5,20,0x000000);
    
    // write battery percentage level
    Lcd.draw_number((xFinal/2)-15, (yFinal/2)+25, LBattery.level(), 0xffff00, 0); 
    
    // fill battery icon from L to R according to battery level
    if (LBattery.level()==0){
      draw_rect((xFinal/2)-26,(yFinal/2)-11,52,22,0xffff00);
    }
    if (LBattery.level()==33){
      draw_rect((xFinal/2)-9,(yFinal/2)-11,35,22,0xffff00);
    }
    if (LBattery.level()==66){
      draw_rect((xFinal/2)+9,(yFinal/2)-11,17,22,0xffff00);
    }
    if (LBattery.level()==100){}
    
    // check if battery is charging
    while(LBattery.isCharging()){
      Lcd.draw_font((xFinal/4),(yFinal/4),"Battery charging\r\n", 0xffff00, 0);
      // draw charging icon
      draw_rect((xFinal/2)-26,(yFinal/2)-11,52,22,0xffff00);
        float k = 7;
        float h = 20;
        for(unsigned char x=0;x<20;x++){
            k=k-0.7;
            for(float y=0;y<k;y++){
              Lcd.draw_point(x+(xFinal/2),y+(yFinal/2),0x000000);
            }
          }
        for(unsigned char x=0;x<20;x++){
            h=h-0.7;
            for(float y=12;y>h;y--){
              Lcd.draw_point(x+((xFinal/2)-17),y+((yFinal/2)-12),0x000000);
            }
          }
          
        // write battery percentage level
        Lcd.draw_number((xFinal/2)-15, (yFinal/2)+25, LBattery.level(), 0xffff00, 0); 
        
        // code to print on Serial Monitor
        int batteryLlevel;
        
        batteryLlevel = LBattery.level();
        Serial.print("Battery level is ");
        Serial.println(batteryLlevel);  
        Serial.println("Battery in charging state.\r\n");
        delay(1000);
    } 
    
    // update Display   
    Lcd.draw_updata();
}
