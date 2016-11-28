#include <LDisplay.h>

void setup() {
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffff00);
}

void loop() {
  Lcd.draw_fill_rectangle(30,30,130,130,0x0000f0);
  Lcd.draw_font(50, 50, "X", 0x0000f0, 0xffffff);
  Lcd.draw_font(50, 90, "Y", 0x0000f0, 0xffffff);
//  Lcd.draw_number(70, 50, 10, 0x0000f0, 0xffffff);
//  Lcd.draw_number(100, 50, 20, 0x0000f0, 0xffffff);
//  Lcd.draw_updata();
}
