

#include "LDisplayExt.h"
#include "Display.h"


void LDisplayExtClass::init(void)
{
  remoteCall(lcd_ext_init, NULL);
}

void LDisplayExtClass::font_init(void)
{
  remoteCall(lcd_ext_font_init, NULL);
}

void LDisplayExtClass::back_light_level(uint8_t ulValue)
{
  remoteCall(lcd_ext_back_light_level, &ulValue);
}

void LDisplayExtClass::screen_set(uint32_t ulValue)
{
  remoteCall(lcd_ext_screen_set, &ulValue);
}

void LDisplayExtClass::draw_font(uint8_t ulX, uint8_t ulY, const char* str, uint32_t BCulValue, uint32_t FCulValue, uint8_t ulSize)
{
  string_info_struct data;
  data.ulX = ulX;
  data.ulY = ulY;
  data.str = (VMCHAR*)str;
  data.BCulValue = BCulValue;
  data.FCulValue = FCulValue;
  data.ulSize = ulSize;
  remoteCall(lcd_ext_draw_font, &data);
}

void LDisplayExtClass::draw_number(uint8_t ulX, uint8_t ulY, uint32_t ulData, uint32_t BCulValue, uint32_t FCulValue, uint8_t ulSize)
{
  number_info_struct data;  
  data.ulX = ulX;
  data.ulY = ulY;
  data.ulData = ulData;
  data.BCulValue = BCulValue;
  data.FCulValue = FCulValue;
  data.ulSize = ulSize;
  remoteCall(lcd_ext_draw_number, &data);
}

void LDisplayExtClass::draw_point(uint8_t ulX, uint8_t ulY, uint32_t PCulValue)
{
  point_info_struct data; 
  data.ulX = ulX;
  data.ulY = ulY;
  data.PCulValue = PCulValue;
  remoteCall(lcd_ext_draw_point, &data);
}

void LDisplayExtClass::draw_line(uint8_t ulX1, uint8_t ulY1, uint8_t ulX2, uint8_t ulY2, uint32_t LCulValue)
{
  line_info_struct data;  
  data.ulX1 = ulX1;
  data.ulY1 = ulY1;
  data.ulX2 = ulX2;
  data.ulY2 = ulY2;
  data.LCulValue = LCulValue;
  remoteCall(lcd_ext_draw_line, &data);
}

// void LDisplayExtClass::draw_fill_rectangle(uint8_t ulX1, uint8_t ulY1, uint8_t ulX2, uint8_t ulY2, uint32_t FCulValue)
// {
//   fill_info_struct data;  
//   data.ulX1 = ulX1;
//   data.ulY1 = ulY1;
//   data.ulX2 = ulX2;
//   data.ulY2 = ulY2;
//   data.FCulValue = FCulValue;
//   remoteCall(lcd_ext_draw_fill_rectangle, &data);
// }
void LDisplayExtClass::draw_fill_rectangle(uint8_t ulX, uint8_t ulY, uint8_t ulWidth, uint8_t ulHeight, uint32_t FCulValue, uint32_t BorderCulValue)
{
  fill_rectangle_struct data;  
  data.ulX = ulX;
  data.ulY = ulY;
  data.ulWidth = ulWidth;
  data.ulHeight = ulHeight;
  data.FCulValue = FCulValue;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_fill_rectangle, &data);
}
void LDisplayExtClass::draw_empty_rectangle(uint8_t ulX, uint8_t ulY, uint8_t ulWidth, uint8_t ulHeight, uint32_t BorderCulValue)
{
  empty_rectangle_struct data;  
  data.ulX = ulX;
  data.ulY = ulY;
  data.ulWidth = ulWidth;
  data.ulHeight = ulHeight;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_empty_rectangle, &data);
}

void LDisplayExtClass::draw_fill_ellipse(uint8_t ulX, uint8_t ulY, uint8_t ulWidth, uint8_t ulHeight, uint32_t FCulValue, uint32_t BorderCulValue)
{
  fill_ellipse_struct data;  
  data.ulX = ulX;
  data.ulY = ulY;
  data.ulWidth = ulWidth;
  data.ulHeight = ulHeight;
  data.FCulValue = FCulValue;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_fill_ellipse, &data);
}
void LDisplayExtClass::draw_empty_ellipse(uint8_t ulX, uint8_t ulY, uint8_t ulWidth, uint8_t ulHeight, uint32_t BorderCulValue)
{
  empty_ellipse_struct data;  
  data.ulX = ulX;
  data.ulY = ulY;
  data.ulWidth = ulWidth;
  data.ulHeight = ulHeight;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_empty_ellipse, &data);
}

void LDisplayExtClass::draw_fill_triangle(uint8_t ulX1, uint8_t ulY1, uint8_t ulX2, uint8_t ulY2, uint8_t ulX3, uint8_t ulY3, uint32_t FCulValue, uint32_t BorderCulValue)
{
  fill_triangle_struct data;
  data.ulX1 = ulX1;
  data.ulY1 = ulY1;
  data.ulX2 = ulX2;
  data.ulY2 = ulY2;
  data.ulX3 = ulX3;
  data.ulY3 = ulY3;
  data.FCulValue = FCulValue;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_fill_triangle, &data);
}

void LDisplayExtClass::draw_empty_triangle(uint8_t ulX1, uint8_t ulY1, uint8_t ulX2, uint8_t ulY2, uint8_t ulX3, uint8_t ulY3, uint32_t BorderCulValue)
{
  empty_triangle_struct data;
  data.ulX1 = ulX1;
  data.ulY1 = ulY1;
  data.ulX2 = ulX2;
  data.ulY2 = ulY2;
  data.ulX3 = ulX3;
  data.ulY3 = ulY3;
  data.BorderCulValue = BorderCulValue;
  remoteCall(lcd_ext_draw_empty_triangle, &data);
}

// void LDisplayClass::draw_fill_polygon(uint8_t points[], uint8_t number_of_points, uint32_t FCulValue)
// {
//  polygon_struct data;  
//  data.points[] = points[];
//  data.number_of_points = number_of_points;
//  // data.ulY = ulY;
//  // data.ulWidth = ulWidth;
//  // data.ulHeight = ulHeight;
//  data.FCulValue = FCulValue;
//  // data.BCulValue = BCulValue;
//  remoteCall(lcd_ext_draw_fill_polygon, &data);
// }

void LDisplayExtClass::draw_updata(void)
{
  remoteCall(lcd_ext_draw_updata, NULL);
}

LDisplayExtClass Lcd;