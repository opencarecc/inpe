

#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif


boolean lcd_ext_init(void* user_data);
boolean lcd_ext_font_init(void* user_data);
boolean lcd_ext_back_light_level(void* user_data);
boolean lcd_ext_screen_set(void* user_data);
boolean lcd_ext_draw_font(void* user_data);
boolean lcd_ext_draw_number(void* user_data);
boolean lcd_ext_draw_point(void* user_data);
boolean lcd_ext_draw_line(void* user_data);
boolean lcd_ext_draw_fill_rectangle(void* user_data);
boolean lcd_ext_draw_empty_rectangle(void* user_data);
boolean lcd_ext_draw_fill_ellipse(void* user_data);
boolean lcd_ext_draw_empty_ellipse(void* user_data);
boolean lcd_ext_draw_fill_triangle(void* user_data);
boolean lcd_ext_draw_empty_triangle(void* user_data);
// boolean lcd_ext_draw_solid_polygon(void* user_data);
boolean lcd_ext_draw_updata(void* user_data);

typedef struct
{
    VMUINT8 ulX;
  VMUINT8 ulY;
    VMCHAR *str;
  VMUINT32 BCulValue;
  VMUINT32 FCulValue;
  VMUINT8 ulSize;
}string_info_struct;

typedef struct
{
    VMUINT8 ulX;
  VMUINT8 ulY;
    VMUINT32 ulData;
  VMUINT32 BCulValue;
  VMUINT32 FCulValue;
  VMUINT8 ulSize;
}number_info_struct;

typedef struct
{
    VMUINT8 ulX;
  VMUINT8 ulY;
  VMUINT32 PCulValue;
}point_info_struct;

typedef struct
{
    VMUINT8 ulX1;
  VMUINT8 ulY1;
  VMUINT8 ulX2;
  VMUINT8 ulY2;
  VMUINT32 LCulValue;
}line_info_struct;

// typedef struct
// {
//     VMUINT8 ulX1;
//   VMUINT8 ulY1;
//   VMUINT8 ulX2;
//   VMUINT8 ulY2;
//   VMUINT32 FCulValue;
// }fill_info_struct;

typedef struct
{
  VMUINT8 ulX;
  VMUINT8 ulY;
  VMUINT8 ulWidth;
  VMUINT8 ulHeight;
  VMUINT32 FCulValue;
  VMUINT32 BorderCulValue;
}fill_rectangle_struct;

typedef struct
{
  VMUINT8 ulX;
  VMUINT8 ulY;
  VMUINT8 ulWidth;
  VMUINT8 ulHeight;
  VMUINT32 BorderCulValue;
}empty_rectangle_struct;

typedef struct
{
  VMUINT8 ulX;
  VMUINT8 ulY;
  VMUINT8 ulWidth;
  VMUINT8 ulHeight;
  VMUINT32 FCulValue;
  VMUINT32 BorderCulValue;
}fill_ellipse_struct;

typedef struct
{
  VMUINT8 ulX;
  VMUINT8 ulY;
  VMUINT8 ulWidth;
  VMUINT8 ulHeight;
  VMUINT32 BorderCulValue;
}empty_ellipse_struct;

typedef struct
{
  VMUINT8 ulX1;
  VMUINT8 ulY1;
  VMUINT8 ulX2;
  VMUINT8 ulY2;
  VMUINT8 ulX3;
  VMUINT8 ulY3;
  VMUINT32 FCulValue;
  VMUINT32 BorderCulValue;
}fill_triangle_struct;

typedef struct
{
  VMUINT8 ulX1;
  VMUINT8 ulY1;
  VMUINT8 ulX2;
  VMUINT8 ulY2;
  VMUINT8 ulX3;
  VMUINT8 ulY3;
  VMUINT32 BorderCulValue;
}empty_triangle_struct;

// typedef struct
// {
//  VMUINT8* points[];
//  VMUINT8 number_of_points;
//  VMUINT32 FCulValue;
// }fill_polygon_struct;


#ifdef __cplusplus
}
#endif

#endif