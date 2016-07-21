// GPS to LCD for RePhone Kit
//
// Hello, this sketch will show the data
//   - coming from the Xadow GPSv2 module
//   - to the Xadow 1.54" Touchscreen v1.0 module
//   - via the Xadow GSM+BLE module
//
// It will hopefully make your GPS module debugging more convenient!
// It's been adapted from the official "gps_test" sketch included
// in the Arduino IDE for RePhone https://github.com/Seeed-Studio/Arduino_IDE_for_RePhone
// which prints the data to the Serial Monitor instead.
//
// Alessandro Contini for http://opencare.cc/
// July 21st, 2016


#include <LGPS.h>
#include <stdio.h>

#include <LDisplay.h>

void setup()
{
//    Serial.begin(115200);
//    Serial.print("GPS test.\r\n");
    
    Lcd.init();
    Lcd.font_init();
    Lcd.back_light_level(50);
    Lcd.screen_set(0xffff00);
    Lcd.draw_font(10, 10, "GPS test", 0xffff00, 0);
}

void loop()
{
    unsigned char *utc_date_time = 0;
    char buffer[50] = {0,};
    
    if(LGPS.check_online())
    {
        utc_date_time = LGPS.get_utc_date_time();
        sprintf(buffer, "UTC:%d-%d-%d  %d:%d:%d\r\n", utc_date_time[0], utc_date_time[1], utc_date_time[2], utc_date_time[3], utc_date_time[4],utc_date_time[5]);
//        Serial.print(buffer);
        Lcd.draw_font(10, 40, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Status %c\r\n", LGPS.get_status());
//        Serial.print(buffer);
        Lcd.draw_font(10, 60, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Lat %c:%f\r\n", LGPS.get_ns(), LGPS.get_latitude());
//        Serial.print(buffer);
        Lcd.draw_font(10, 80, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Long %c:%f\r\n", LGPS.get_ew(), LGPS.get_longitude());
//        Serial.print(buffer);
        Lcd.draw_font(10, 100, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Alt %f\r\n", LGPS.get_altitude());
//        Serial.print(buffer);
        Lcd.draw_font(10, 120, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Speed %f\r\n", LGPS.get_speed());
//        Serial.print(buffer);
        Lcd.draw_font(10, 140, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Sats %d\r\n", LGPS.get_sate_used());
//        Serial.print(buffer);
        Lcd.draw_font(10, 160, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Course %f\r\n", LGPS.get_course());
//        Serial.print(buffer);
        Lcd.draw_font(10, 180, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Position fix %c\r\n", LGPS.get_position_fix());
//        Serial.print(buffer);
        Lcd.draw_font(10, 200, buffer, 0xffff00, 0);
        
        sprintf(buffer, "Mode %c   Mode2 %c\r\n\r\n", LGPS.get_mode(), LGPS.get_mode2());
//        Serial.print(buffer);
        Lcd.draw_font(10, 220, buffer, 0xffff00, 0);
    }    
    delay(1000);
}
