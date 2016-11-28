#include <LDisplay.h>
#include "LTask.h"
#include "vmgraphic_font.h"

//int font_size=40;

//boolean setFontSize(void* userdata){
//  vm_graphic_set_font_size(40);
//  return true;
//}

void setup() {
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffff00);
//  LTask.remoteCall(setFontSize,NULL);
}

void loop() {
//  Lcd.draw_fill_rectangle(30,30,130,130,0x0000f0);
//  Lcd.draw_number(70, 50, 10, 0x0000f0, 0xffffff);
//  Lcd.draw_updata();

Lcd.draw_font(10, 10, "A", 0x0000f0, 0xffffff, 40);
//Lcd.draw_font(25, 10, "B", 0x0000f0, 0xffffff, 20);
//Lcd.draw_font(10, 40, "a", 0x0000f0, 0xffffff, 20);
//Lcd.draw_font(25, 40, "b", 0x0000f0, 0xffffff, 20);
//Lcd.draw_font(10, 70, "ab", 0x0000f0, 0xffffff, 20);
Lcd.draw_number(70, 50, 1001, 0x0000f0, 0xffffff, 20);
}
