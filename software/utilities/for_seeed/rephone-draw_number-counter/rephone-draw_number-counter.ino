#include <LDisplay.h>

void setup() {
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffff00);
}

void loop() {
  Lcd.draw_fill_rectangle(30,30,130,130,0x0000f0);
  for(int i;i<=100;i++){
    Lcd.draw_number(70, 50, i, 0x0000f0, 0xffffff);
    delay(500);
    if(i==100){
      i=0;
    }
  }
//  Lcd.draw_updata();
}
