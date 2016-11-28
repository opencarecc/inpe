#include <LDisplay.h>

int i;

void setup() {
  Serial.begin(115200);
  Lcd.init();
  Lcd.font_init();
  Lcd.back_light_level(50);
  Lcd.screen_set(0xffff00);
}

void loop() {
  Lcd.draw_fill_rectangle(30,30,130,130,0x0000f0);
  i = random(0, 100);
  Lcd.draw_number(70, 50, i, 0x0000f0, 0xffffff);
  Serial.println(i);
  delay(500);
//  Lcd.draw_updata();
}
