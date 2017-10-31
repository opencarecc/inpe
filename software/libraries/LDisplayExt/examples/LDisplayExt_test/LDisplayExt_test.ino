/*
  Extended Display library for Rephone Kit
  
  Hello, this sketch will show on screen all the functions of the
  Extended Display library for Rephone Kit.

  HARDWARE:
  - Xadow GSM+BLE
  - Xadow 1.54" Touchscreen
  - Lipo battery 3.7v 520mAh
  
  by Alessandro Contini @ WeMake for http://opencare.cc/
  Based on work by Wayne Weng at https://www.seeed.cc/
  2017
*/

#include <LDisplayExt.h>

unsigned long counter = 0;

void setup()
{
    Serial.begin(115200);
    Serial.print("LCD display test.\r\n");

    /*** INITIALIZE ***/
      Lcd.init();   // Initialize LCD screen
      Lcd.font_init();  // Initialize font
      Lcd.back_light_level(100);  // Set brightness to max (min value 0 - max value 100)
      Lcd.screen_set(0xffff00);   // Set background color, accepts hex color value

    /*** DRAW TEXT ***/
      // Lcd.draw_font function attributes are
      // (X position, Y position, "Your text", Background color, Font color, Font size)
      Lcd.draw_font(0, 1, " LDisplayExt Test ", 0xff00ff, 0, 20); // Highlighted text
      Lcd.draw_font(0, 30, "Font size: 10", 0xffff00, 0, 10);   // Text with font size 10
      Lcd.draw_font(0, 45, "Font size: 20", 0xffff00, 0, 20);   // Text with font size 20

    /*** DRAW ICON ***/
      // Lcd.draw_point function attributes are
      // (X position, Y position, color)
      Lcd.draw_font(0, 70, "Draw icon:", 0xffff00, 0, 20);
      float k = 7;
      float h = 20;
      for(unsigned char x=0;x<20;x++){
          k=k-0.7;
          for(float y=0;y<k;y++){
            Lcd.draw_point(x+110,y+80,0x000000);
          }
        }
      for(unsigned char x=0;x<20;x++){
          h=h-0.7;
          for(float y=12;y>h;y--){
            Lcd.draw_point(x+(110-17),y+(80-12),0x000000);
          }
        }
    
    /***  DRAW LINES ***/ 
      // Lcd.draw_line function attributes are
      // (X start point, Y start point, X end point, Y end point, color)
      for(int l=230; l<240; l=l+3){
        Lcd.draw_line(0,l,240,l,0x000000);  //  Draw horizontal parallel lines at the bottom of the screen
      }   
      for(int l=240; l>200; l=l-3){
        Lcd.draw_line(240,0,l,240,0x00ff00);  //  Draw vertical lines on right end of the screen
      }
    
    /***  DRAW RECTANGLES ***/
      // Lcd.draw_fill_rectangle function attributes are
      // (X position, Y position, Width, Height, Fill color, Border color)
      Lcd.draw_fill_rectangle(10,100,20,20,0xff0000,0x000000);  // Draw solid rectangle with border
      Lcd.draw_fill_rectangle(40,100,20,20,0xff0000,0xff0000);  // Draw solid rectangle without border
      
      // Lcd.draw_empty_rectangle function attributes are
      // (X position, Y position, Width, Height, Border color)
      Lcd.draw_empty_rectangle(70,100,20,20,0xff0000);
      Lcd.draw_empty_rectangle(75,105,20,20,0xff00ff);

    /***  DRAW ELLIPSES ***/
      // Lcd.draw_fill_ellipse function attributes are
      // (X position, Y position, Width, Height, Fill color, Border color)
      Lcd.draw_fill_ellipse(10,140,20,20,0xff0000,0x000000);  // Draw solid ellipse with border
      Lcd.draw_fill_ellipse(40,140,20,20,0xff0000,0xff0000);  // Draw solid ellipse without border
      
      // Lcd.draw_empty_ellipse function attributes are
      // (X position, Y position, Width, Height, Border color)
      Lcd.draw_empty_ellipse(70,140,20,20,0xff0000);
      Lcd.draw_empty_ellipse(75,145,20,20,0xff00ff);

    /***  DRAW TRIANGLES ***/
      // Lcd.draw_fill_triangle function attributes are
      // (X1 point, Y1 point, X2 point, Y2 point, X3 point, Y3 point, Fill color, Border color)
      Lcd.draw_fill_triangle(10,180,30,200,10,200,0xff0000,0x000000);   // Draw solid triangle with border
      Lcd.draw_fill_triangle(40,180,60,180,40,200,0xff0000,0xff0000);   // Draw solid triangle without border
  
      // Lcd.draw_fill_triangle function attributes are
      // (X1 point, Y1 point, X2 point, Y2 point, X3 point, Y3 point, Border color)
      Lcd.draw_empty_triangle(80,180,90,200,70,200,0xff0000);
      Lcd.draw_empty_triangle(85,185,95,205,75,205,0xff00ff);

    /***  UPDATE LCD ***/
      Lcd.draw_updata();
}

void loop()
{
    counter++;    // Add 1 to counter at every loop
    if(counter >= 100)counter = 0;  // When counter gets to 99 it will go back to 0 
    Lcd.draw_fill_rectangle(115,100,92,64,0xffff00,0x000000);   // Draw background rectangle to fix left alignment behavior
    Lcd.draw_number(120, 108, counter, 0xffff00, 0, 64);    // Draw current counter value
    Lcd.draw_font(190, 108, "*", 0xffff00, 0xff0000, 32);   // Draw symbol
    Lcd.draw_updata();    // Update LCD at every loop
    delay(1000);
}

