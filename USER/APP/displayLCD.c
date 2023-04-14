#include "displayLCD.h"
#include "usart.h"
#include "ST7567a.h"
#include "rtc.h"

unsigned char icon_test[][16]= {
{0xF8,0xFC,0x0C,0x7C,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x7C,0x0C,0x0C,0xFC,0xF8},
{0xF0,0xF8,0xFC,0xFC,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xFC,0xFC,0xFC,0xF8,0xF0},
};


unsigned char icon_4G[][16]= {
0x00,0x18,0x38,0x68,0x88,0xF8,0x88,0x68,0x38,0x18,0x80,0x00,0xE0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x3C,0x00,0x3F,0x00,0x3F,0x00,0x00,0x00,/*"C:\Users\86156\Desktop\4G_16x16.bmp",0*/
};

unsigned char icon_beidou[][16]= {
0x00,0xF0,0x18,0x04,0x02,0x82,0xE1,0xA1,0xB1,0x71,0x32,0x02,0x04,0x18,0xF0,0x00,
0x00,0x01,0x03,0x04,0x08,0x09,0x11,0x11,0x10,0x10,0x08,0x08,0x04,0x03,0x01,0x00,
};

unsigned char icon_ppl[][16]= {
0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE,0xFE,0xFE,0xFE,0xFC,0xF8,0x00,0x00,0x00,0x00,
0x78,0x7C,0x7C,0x7C,0x7C,0x7D,0x7F,0x7F,0x7F,0x7F,0x7D,0x7C,0x7C,0x7C,0x7C,0x78,
};

unsigned char icon_camera[][16]= {
{0x00,0xF8,0x04,0x02,0xE1,0x11,0x09,0xC9,0xC9,0x09,0x11,0xE1,0x02,0x04,0xF8,0x00},
{0xC0,0xF3,0xF4,0xEC,0xD8,0xD1,0x92,0x92,0x92,0x92,0xD1,0xD8,0xEC,0xF4,0xF3,0xC0},/*"δ�����ļ�",0*/};

unsigned char icon_SD[][16]= {
{0x00,0xF8,0x04,0x02,0x39,0x01,0xB9,0x81,0x39,0x81,0xB9,0x01,0x39,0x01,0xFF,0x00},
{0x00,0xFF,0x80,0x80,0x80,0x8B,0x8A,0x86,0x80,0x8F,0x88,0x87,0x80,0x80,0xFF,0x00},
};

unsigned char icon_charged[][16]= {
{0x00,0xF8,0x08,0x88,0x88,0x88,0x88,0xC8,0x28,0xE8,0x48,0x48,0x08,0x78,0x40,0xC0},
{0x00,0x1F,0x10,0x11,0x11,0x11,0x11,0x13,0x14,0x17,0x12,0x12,0x10,0x1E,0x02,0x03},
};

unsigned char icon_battery[][16]= {
{0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x40,0xC0},
{0x00,0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x02,0x03},
};

unsigned char empty[][24]= {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

unsigned char Chinese_car_plate[][24]= {
{0x88,0x68,0xFF,0x28,0x40,0xFE,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00,0x0F,0x08,0x04,0x03,0x00,0x00,0x07,0x08,0x0E,0x00},/*"��",0*/
{0x10,0xD2,0x32,0x92,0x10,0x00,0x08,0xFF,0x08,0x08,0xF8,0x00,0x03,0x02,0x02,0x02,0x03,0x08,0x06,0x01,0x08,0x08,0x07,0x00},/*"��",1*/
{0x04,0x64,0x54,0x4C,0x47,0xF4,0x44,0x44,0x44,0x44,0x04,0x00,0x02,0x02,0x02,0x02,0x02,0x0F,0x02,0x02,0x02,0x02,0x02,0x00},/*"��",2*/
{0x20,0x20,0xAF,0x69,0x29,0x29,0x29,0x29,0x2F,0x20,0x20,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x09,0x09,0x07,0x00,0x00},/*"��",3*/
{0x00,0xFE,0x90,0x9F,0x10,0x7C,0xD6,0x75,0x5C,0x54,0x7C,0x00,0x08,0x07,0x00,0x0F,0x02,0x03,0x02,0x02,0x0F,0x02,0x02,0x00},/*"��",4*/
{0x20,0x10,0x2C,0x23,0xE0,0x20,0x20,0x23,0xEC,0x10,0x20,0x00,0x00,0x08,0x04,0x03,0x00,0x08,0x08,0x08,0x07,0x00,0x00,0x00},/*"��",5*/
{0x08,0x48,0x2A,0x1C,0x08,0xBF,0x08,0x1C,0x2A,0x48,0x08,0x00,0x09,0x09,0x05,0x05,0x03,0x01,0x03,0x05,0x05,0x09,0x09,0x00},/*"��",6*/
{0x12,0x4A,0xC7,0x52,0x52,0x4E,0x40,0x5E,0xD2,0x12,0x1E,0x00,0x04,0x04,0x05,0x05,0x05,0x05,0x05,0x01,0x09,0x09,0x07,0x00},/*"��",7*/
{0x02,0x7A,0x42,0x7E,0xC0,0x80,0x7C,0x44,0xFF,0x44,0x7C,0x00,0x02,0x02,0x09,0x08,0x07,0x08,0x05,0x02,0x05,0x08,0x08,0x00},/*"ʻ",8*/
{0x11,0xF2,0x00,0x02,0xE2,0x02,0x02,0xFE,0x42,0x42,0x42,0x00,0x00,0x07,0x02,0x09,0x0F,0x08,0x08,0x0F,0x08,0x08,0x08,0x00},/*"֤",9*/
{0x20,0x20,0xAF,0x69,0x29,0x29,0x29,0x29,0x2F,0x20,0x20,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x09,0x09,0x07,0x00,0x00},/*"��",10*/
{0x42,0xF2,0x2E,0xE2,0x01,0x3D,0x21,0x21,0x21,0x3F,0xE0,0x00,0x00,0x07,0x02,0x07,0x01,0x01,0x01,0x01,0x09,0x08,0x07,0x00},/*"��",11*/
};

unsigned char screen_clear[][16]= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


//unsigned char need_break[][24]= {
//{0x8C,0xA5,0xAD,0xAD,0x85,0xBF,0x85,0xAD,0xAD,0xA5,0x8C,0x00,0x00,0x0E,0x02,0x02,0x0E,0x03,0x0E,0x02,0x0A,0x0E,0x00,0x00},/*"��",0*/
//{0x82,0xBA,0xAA,0xAA,0xFE,0xAA,0xBE,0xAA,0xAA,0xBA,0x82,0x00,0x08,0x08,0x0A,0x0B,0x04,0x04,0x04,0x0A,0x09,0x08,0x00,0x00},/*"Ҫ",1*/
//{0x20,0x10,0xFC,0x03,0x08,0x88,0x68,0xFF,0x68,0x88,0x08,0x00,0x00,0x00,0x0F,0x00,0x02,0x01,0x00,0x0F,0x00,0x01,0x02,0x00},/*"��",2*/
//{0x00,0xFE,0xAA,0xAA,0xAB,0xAA,0xAA,0xAA,0xAA,0xFE,0x00,0x00,0x08,0x06,0x00,0x06,0x08,0x09,0x0A,0x08,0x0C,0x02,0x0C,0x00},/*"Ϣ",3*/
//};

unsigned char vehicle_driver_info[][24]= {
{0x04,0x64,0x54,0x4C,0x47,0xF4,0x44,0x44,0x44,0x44,0x04,0x00,0x02,0x02,0x02,0x02,0x02,0x0F,0x02,0x02,0x02,0x02,0x02,0x00},/*"��",0*/
{0x34,0x2C,0xF7,0xA4,0xF2,0x12,0xFE,0x12,0xFE,0x12,0xF2,0x00,0x01,0x01,0x0F,0x00,0x0F,0x01,0x00,0x01,0x00,0x09,0x0F,0x00},/*"��",1*/
{0x00,0x02,0xC2,0x3E,0x62,0x82,0x02,0x32,0x2A,0xA6,0x60,0x00,0x08,0x06,0x01,0x08,0x08,0x04,0x05,0x02,0x05,0x08,0x08,0x00},/*"��",2*/
{0x12,0x4A,0xC7,0x52,0x52,0x4E,0x40,0x5E,0xD2,0x12,0x1E,0x00,0x04,0x04,0x05,0x05,0x05,0x05,0x05,0x01,0x09,0x09,0x07,0x00},/*"��",3*/
{0x02,0x7A,0x42,0x7E,0xC0,0x80,0x7C,0x44,0xFF,0x44,0x7C,0x00,0x02,0x02,0x09,0x08,0x07,0x08,0x05,0x02,0x05,0x08,0x08,0x00},/*"ʻ",4*/
{0x00,0xF0,0x17,0x15,0x15,0xD5,0x15,0x15,0x17,0xF0,0x00,0x00,0x08,0x09,0x04,0x04,0x02,0x01,0x02,0x02,0x04,0x05,0x08,0x00},/*"Ա",5*/
{0x10,0xFC,0x03,0x04,0x54,0x54,0x55,0x56,0x54,0x54,0x04,0x00,0x00,0x0F,0x00,0x00,0x0F,0x05,0x05,0x05,0x05,0x0F,0x00,0x00},/*"��",6*/
{0x00,0xFE,0xAA,0xAA,0xAB,0xAA,0xAA,0xAA,0xAA,0xFE,0x00,0x00,0x08,0x06,0x00,0x06,0x08,0x09,0x0A,0x08,0x0C,0x02,0x0C,0x00},/*"Ϣ",7*/
};

unsigned char overTimeDriveRecord[][24]= {
{0x20,0x22,0x22,0x22,0xE2,0x3E,0xE2,0x22,0x22,0x22,0x20,0x00,0x08,0x04,0x02,0x01,0x00,0x00,0x07,0x08,0x08,0x08,0x0E,0x00},/*"��",0*/
{0x20,0xA4,0x24,0xFF,0x24,0x20,0xD2,0x4E,0x42,0x52,0xDE,0x00,0x08,0x07,0x04,0x0F,0x09,0x09,0x0B,0x0A,0x0A,0x0A,0x0B,0x00},/*"��",1*/
{0xFE,0x22,0x22,0xFE,0x00,0x08,0x48,0x88,0x08,0xFF,0x08,0x00,0x07,0x02,0x02,0x07,0x00,0x00,0x00,0x09,0x08,0x0F,0x00,0x00},/*"ʱ",2*/
{0x21,0xE2,0x00,0x04,0x34,0x2C,0x27,0xF4,0x24,0x24,0x04,0x00,0x08,0x07,0x08,0x09,0x09,0x09,0x09,0x0F,0x09,0x09,0x09,0x00},/*"��",3*/
{0x98,0xD4,0xB3,0x88,0x00,0x4A,0xAA,0x4F,0xEA,0x0A,0x18,0x00,0x04,0x04,0x02,0x02,0x09,0x09,0x05,0x03,0x01,0x05,0x09,0x00},/*"��",4*/
{0x12,0x4A,0xC7,0x52,0x52,0x4E,0x40,0x5E,0xD2,0x12,0x1E,0x00,0x04,0x04,0x05,0x05,0x05,0x05,0x05,0x01,0x09,0x09,0x07,0x00},/*"��",5*/
{0x02,0x7A,0x42,0x7E,0xC0,0x80,0x7C,0x44,0xFF,0x44,0x7C,0x00,0x02,0x02,0x09,0x08,0x07,0x08,0x05,0x02,0x05,0x08,0x08,0x00},/*"ʻ",6*/
{0x10,0x11,0xF2,0x00,0x00,0xE2,0x22,0x22,0x22,0x3E,0x00,0x00,0x00,0x00,0x07,0x02,0x01,0x07,0x08,0x08,0x08,0x08,0x0E,0x00},/*"��",7*/
{0x10,0x51,0x95,0x15,0x95,0xF5,0x95,0x15,0x9F,0x50,0x10,0x00,0x04,0x04,0x02,0x01,0x08,0x0F,0x00,0x01,0x02,0x04,0x04,0x00},/*"¼",8*/
};

unsigned char pulseRatio[][24]= {
{0x00,0xFE,0x92,0xFE,0x20,0xE9,0x09,0xFA,0x62,0x90,0x08,0x00,0x08,0x07,0x08,0x0F,0x02,0x01,0x08,0x0F,0x00,0x01,0x02,0x00},/*"��",0*/
{0x02,0x04,0x80,0x00,0xFC,0x84,0x84,0xFF,0x84,0x84,0xFC,0x00,0x02,0x01,0x00,0x00,0x01,0x00,0x00,0x0F,0x00,0x00,0x01,0x00},/*"��",1*/
{0x00,0x82,0x92,0xDA,0xD6,0xB2,0xB1,0x91,0x89,0xC1,0x80,0x00,0x08,0x04,0x02,0x00,0x08,0x0F,0x00,0x00,0x02,0x04,0x09,0x00},/*"ϵ",2*/
{0x48,0x2A,0x98,0x7F,0x28,0x4A,0x10,0xEF,0x08,0xF8,0x08,0x00,0x09,0x0B,0x05,0x05,0x0B,0x00,0x08,0x05,0x02,0x05,0x08,0x00},/*"��",3*/
};


unsigned char over_speed[][24]= {
{0x20,0xA4,0x24,0xFF,0x24,0x20,0xD2,0x4E,0x42,0x52,0xDE,0x00,0x08,0x07,0x04,0x0F,0x09,0x09,0x0B,0x0A,0x0A,0x0A,0x0B,0x00},/*"��",0*/
{0x11,0xF2,0x00,0x7A,0x4A,0xCA,0xFF,0xCA,0x4A,0x7A,0x02,0x00,0x08,0x07,0x08,0x0A,0x09,0x08,0x0F,0x08,0x09,0x0A,0x08,0x00},/*"��",1*/
{0x12,0x4A,0xC7,0x52,0x52,0x4E,0x40,0x5E,0xD2,0x12,0x1E,0x00,0x04,0x04,0x05,0x05,0x05,0x05,0x05,0x01,0x09,0x09,0x07,0x00},/*"��",2*/
{0x02,0x7A,0x42,0x7E,0xC0,0x80,0x7C,0x44,0xFF,0x44,0x7C,0x00,0x02,0x02,0x09,0x08,0x07,0x08,0x05,0x02,0x05,0x08,0x08,0x00},/*"ʻ",3*/
};

unsigned char abnormal_speed[][24]= {
{0x11,0xF2,0x00,0x7A,0x4A,0xCA,0xFF,0xCA,0x4A,0x7A,0x02,0x00,0x08,0x07,0x08,0x0A,0x09,0x08,0x0F,0x08,0x09,0x0A,0x08,0x00},/*"��",0*/
{0x00,0xFE,0x0A,0x8A,0xBE,0xAA,0xAB,0xAA,0xBE,0x8A,0x0A,0x00,0x08,0x07,0x00,0x08,0x09,0x0A,0x04,0x04,0x0A,0x09,0x08,0x00},/*"��",1*/
{0x00,0x1F,0x25,0xE5,0x25,0x25,0x25,0x25,0xE5,0x27,0x30,0x00,0x01,0x09,0x05,0x03,0x01,0x01,0x01,0x01,0x0F,0x01,0x01,0x00},/*"��",2*/
{0x0C,0x04,0x75,0x56,0x54,0xD7,0x54,0x56,0x75,0x04,0x0C,0x00,0x00,0x07,0x01,0x01,0x01,0x0F,0x01,0x01,0x05,0x07,0x00,0x00},/*"��",3*/
};

unsigned char weekday_array[][24]= {
{0x00,0xDF,0x95,0x95,0x95,0xF5,0x95,0x95,0x95,0x9F,0x00,0x00,0x09,0x08,0x0A,0x0A,0x0A,0x0F,0x0A,0x0A,0x0A,0x08,0x08,0x00},/*"��",0*/
{0x04,0xFF,0x54,0x54,0xFF,0x04,0x00,0xFE,0x92,0x92,0xFE,0x00,0x09,0x05,0x01,0x01,0x05,0x09,0x08,0x07,0x00,0x08,0x0F,0x00},/*"��",1*/
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"һ",2*/
{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00},/*"��",3*/
{0x00,0x02,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x02,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00},/*"��",4*/
{0x00,0xFE,0x02,0x82,0x7E,0x02,0x02,0x7E,0x82,0x82,0xFE,0x00,0x00,0x0F,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00},/*"��",5*/
{0x02,0x22,0x22,0xE2,0x3E,0x22,0x22,0x22,0xE2,0x02,0x00,0x00,0x08,0x08,0x0E,0x09,0x08,0x08,0x08,0x08,0x0F,0x08,0x08,0x00},/*"��",6*/
{0x10,0x10,0x10,0xD0,0x11,0x16,0x10,0x50,0x90,0x10,0x10,0x00,0x08,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,0x00},/*"��",7*/
{0x20,0x22,0x22,0x22,0xA2,0x7E,0xA2,0x22,0x22,0x22,0x20,0x00,0x08,0x08,0x04,0x02,0x01,0x00,0x01,0x02,0x04,0x08,0x08,0x00},/*"��",8*/
{0xFE,0x22,0x22,0xFE,0x00,0x08,0x48,0x88,0x08,0xFF,0x08,0x00,0x07,0x02,0x02,0x07,0x00,0x00,0x00,0x09,0x08,0x0F,0x00,0x00},/*"ʱ",9*/
{0x20,0x10,0x2C,0x23,0xE0,0x20,0x20,0x23,0xEC,0x10,0x20,0x00,0x00,0x08,0x04,0x03,0x00,0x08,0x08,0x08,0x07,0x00,0x00,0x00},/*"��",10*/
{0x12,0xD2,0xFE,0x91,0x40,0x38,0x00,0xFF,0x00,0x04,0xB8,0x00,0x01,0x00,0x0F,0x00,0x08,0x08,0x04,0x04,0x02,0x01,0x00,0x00},/*"��",11*/
};

unsigned char car_plate_color[][24]= {
{0x02,0x72,0x07,0x7A,0x02,0x22,0x1A,0x12,0x37,0x52,0x12,0x00,0x08,0x0F,0x09,0x09,0x0F,0x09,0x0F,0x09,0x09,0x0F,0x08,0x00},/*"��",0*/
{0x08,0xEA,0xAA,0xAF,0xAA,0xFA,0xAA,0xAF,0xAA,0xEA,0x08,0x00,0x00,0x0B,0x06,0x02,0x02,0x03,0x02,0x02,0x06,0x0B,0x00,0x00},/*"��",1*/
{0x00,0x5F,0x51,0x55,0x51,0xFF,0x51,0x55,0x51,0x5F,0x00,0x00,0x09,0x05,0x01,0x05,0x09,0x01,0x05,0x09,0x01,0x05,0x09,0x00},/*"��",2*/
{0x00,0xFC,0x44,0x46,0x45,0x44,0x44,0x44,0x44,0xFC,0x00,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00},/*"��",3*/
{0x08,0xF4,0x92,0x93,0x92,0xF2,0x92,0x9A,0x96,0xF0,0x00,0x00,0x00,0x07,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0E,0x00},/*"ɫ",4*/
{0x20,0x24,0x24,0xA4,0x64,0xFF,0x64,0xA4,0x24,0x24,0x20,0x00,0x02,0x02,0x01,0x00,0x00,0x0F,0x00,0x00,0x01,0x02,0x02,0x00},/*"δ",5*/
{0x00,0x00,0x00,0x00,0xFF,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x08,0x08,0x08,0x08,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x00},/*"��",6*/
{0x00,0xFE,0x90,0x9F,0x10,0x7C,0xD6,0x75,0x5C,0x54,0x7C,0x00,0x08,0x07,0x00,0x0F,0x02,0x03,0x02,0x02,0x0F,0x02,0x02,0x00},/*"��",7*/
{0x04,0x04,0xFF,0x54,0x54,0x54,0x54,0x54,0xFF,0x04,0x04,0x00,0x01,0x09,0x05,0x01,0x01,0x01,0x01,0x01,0x05,0x09,0x01,0x00},/*"��",8*/
{0x20,0x10,0xFC,0x03,0x20,0xFC,0x10,0xFF,0x08,0x84,0xFC,0x00,0x00,0x00,0x0F,0x00,0x00,0x07,0x08,0x09,0x08,0x08,0x0E,0x00},/*"��",9*/
};

unsigned char car_plate_province[][24]= {
{0x04,0xF4,0x94,0x94,0x95,0x96,0x94,0x94,0x94,0xF4,0x04,0x00,0x08,0x04,0x02,0x00,0x08,0x0F,0x00,0x00,0x02,0x04,0x08,0x00},/*"��",0*/
{0x22,0x44,0x00,0x08,0xAA,0xAA,0xFF,0xAA,0xAA,0xBE,0x08,0x00,0x04,0x02,0x01,0x02,0x02,0x02,0x0F,0x02,0x02,0x02,0x02,0x00},/*"��",1*/
{0x10,0x21,0x02,0x00,0xFC,0x44,0x45,0x46,0x44,0x44,0xFC,0x00,0x04,0x02,0x09,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"��",2*/
{0x10,0x21,0x02,0xE8,0xA4,0xAA,0xE9,0x0A,0xC4,0x08,0xE8,0x00,0x04,0x02,0x01,0x0F,0x02,0x0A,0x0F,0x00,0x03,0x08,0x0F,0x00},/*"��",3*/
{0x04,0xA4,0xA4,0xA4,0xA4,0xBF,0xA4,0xA4,0xA4,0xA4,0x04,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00},/*"��",4*/
{0x10,0x11,0xF2,0x00,0x02,0x02,0x02,0xF2,0x12,0x0A,0x06,0x00,0x08,0x04,0x03,0x04,0x08,0x08,0x0A,0x0B,0x08,0x08,0x08,0x00},/*"��",5*/
{0x10,0xD5,0x59,0x51,0x5F,0x51,0x5F,0x51,0x59,0xD5,0x10,0x00,0x00,0x0F,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x0F,0x00,0x00},/*"��",6*/
{0x0A,0x0A,0xFA,0x5F,0x50,0xF0,0x57,0x5A,0xFA,0x09,0x0C,0x00,0x04,0x05,0x0D,0x07,0x05,0x05,0x05,0x07,0x0D,0x05,0x04,0x00},/*"��",7*/
{0x22,0x2A,0xEA,0xAA,0xAA,0xBF,0xAA,0xAA,0xEA,0x2A,0x22,0x00,0x00,0x00,0x0F,0x02,0x02,0x02,0x02,0x0A,0x0F,0x00,0x00,0x00},/*"��",8*/
{0x88,0xFC,0xAB,0xAA,0xAA,0xFA,0xAA,0xAE,0xA8,0xF8,0x80,0x00,0x00,0x00,0x0F,0x0A,0x0A,0x0A,0x0A,0x0A,0x0F,0x00,0x00,0x00},/*"³",9*/
{0x4A,0x52,0xEA,0x46,0xC8,0xBC,0x6B,0xFA,0xAE,0x2A,0xB8,0x00,0x00,0x08,0x0F,0x00,0x0A,0x0A,0x05,0x0A,0x0F,0x03,0x04,0x00},/*"ԥ",10*/
{0x02,0xD2,0x12,0x17,0xFA,0x12,0x12,0x17,0xF2,0x42,0x82,0x00,0x02,0x09,0x04,0x03,0x00,0x08,0x08,0x08,0x07,0x00,0x01,0x00},/*"��",11*/
{0xFC,0x46,0xFD,0x00,0x8C,0xA4,0xA5,0xA6,0xA4,0xA4,0x8C,0x00,0x0F,0x04,0x0F,0x00,0x08,0x04,0x03,0x00,0x07,0x08,0x0C,0x00},/*"��",12*/
{0x11,0x22,0x88,0x88,0xFF,0x48,0x00,0xFE,0x12,0xF2,0x11,0x00,0x04,0x02,0x00,0x08,0x0F,0x00,0x08,0x07,0x00,0x0F,0x00,0x00},/*"��",13*/
{0xF9,0x02,0x78,0x49,0x49,0xFF,0x49,0x49,0x79,0x01,0xFF,0x00,0x0F,0x00,0x02,0x02,0x02,0x03,0x02,0x03,0x06,0x08,0x0F,0x00},/*"��",14*/
{0x0A,0xFE,0xAB,0xFE,0x0A,0x94,0xB7,0xEA,0xAA,0xB6,0x90,0x00,0x02,0x02,0x0F,0x02,0x02,0x0B,0x04,0x02,0x04,0x0B,0x00,0x00},/*"��",15*/
{0x11,0x22,0x88,0x48,0xFF,0x28,0x48,0xFE,0x92,0x92,0xFE,0x00,0x04,0x02,0x00,0x00,0x0F,0x00,0x00,0x0F,0x04,0x04,0x0F,0x00},/*"��",16*/
{0x47,0xD5,0x57,0x50,0x57,0x55,0x47,0x00,0xFF,0x31,0xCF,0x00,0x00,0x01,0x01,0x09,0x09,0x07,0x00,0x00,0x0F,0x02,0x01,0x00},/*"��",17*/
{0x80,0xFE,0x92,0xDA,0x93,0xFE,0x92,0xDA,0x92,0xFE,0x80,0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x02,0x0A,0x0A,0x06,0x00,0x00},/*"��",18*/
{0x22,0x22,0xFE,0x22,0x04,0xF4,0x95,0x96,0x94,0xF4,0x04,0x00,0x04,0x04,0x03,0x02,0x04,0x02,0x08,0x0F,0x00,0x02,0x04,0x00},/*"��",19*/
{0x04,0x04,0xFF,0x44,0x44,0x44,0x44,0x44,0xFF,0x04,0x04,0x00,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},/*"��",20*/
{0xFE,0x02,0x32,0xCE,0x94,0xA4,0x84,0xFF,0x84,0xA4,0x94,0x00,0x0F,0x02,0x02,0x01,0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x00},/*"��",21*/
{0x5F,0x55,0x51,0xFF,0x55,0x7F,0x90,0x8C,0xA3,0x8C,0x90,0x00,0x0A,0x06,0x0A,0x05,0x09,0x05,0x08,0x00,0x08,0x06,0x01,0x00},/*"ǭ",22*/
{0x22,0x44,0x00,0x02,0xFA,0xAA,0xAF,0xAA,0xAA,0xFA,0x02,0x00,0x04,0x02,0x01,0x02,0x0B,0x06,0x02,0x02,0x06,0x0B,0x02,0x00},/*"��",23*/
{0x00,0x00,0xFF,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFF,0x00,0x08,0x04,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x0F,0x00},/*"��",24*/
{0x00,0x5F,0x51,0x55,0x51,0xFF,0x51,0x55,0x51,0x5F,0x00,0x00,0x09,0x05,0x01,0x05,0x09,0x01,0x05,0x09,0x01,0x05,0x09,0x00},/*"��",25*/
{0x1A,0x8A,0xAA,0x6F,0x6A,0xAA,0x2A,0x2F,0xAA,0x4A,0x1A,0x00,0x08,0x0A,0x0A,0x05,0x05,0x0A,0x0F,0x01,0x02,0x04,0x04,0x00},/*"��",26*/
{0x20,0xAA,0xB2,0xE3,0xB2,0xAA,0x00,0xFE,0x12,0xF2,0x11,0x00,0x04,0x02,0x08,0x0F,0x00,0x0A,0x04,0x03,0x00,0x0F,0x00,0x00},/*"��",27*/
{0xBA,0xA2,0xFA,0x0F,0xEA,0xAA,0xEA,0xAF,0xFA,0x0E,0xCA,0x00,0x03,0x08,0x07,0x00,0x0F,0x0A,0x0E,0x0B,0x04,0x07,0x0C,0x00},/*"��",28*/
{0x2C,0x24,0x24,0x24,0x25,0xE6,0x24,0x24,0x24,0x24,0x2C,0x00,0x00,0x00,0x00,0x08,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x00},/*"��",29*/
{0x88,0x68,0xFF,0x48,0x20,0x24,0x24,0xBF,0x24,0x24,0x20,0x00,0x00,0x00,0x0F,0x00,0x08,0x09,0x09,0x0F,0x09,0x09,0x08,0x00},/*"��",30*/
{0x22,0x44,0x90,0x54,0xBF,0x94,0x94,0x94,0xBF,0x54,0x90,0x00,0x08,0x04,0x02,0x00,0x07,0x0A,0x0A,0x0A,0x0B,0x0C,0x00,0x00},/*"��",31*/
{0x11,0x22,0xFE,0x12,0x5A,0x13,0x7E,0x12,0x5A,0x12,0xFE,0x00,0x04,0x02,0x0A,0x0A,0x06,0x02,0x03,0x02,0x06,0x0A,0x0A,0x00},/*"��",32*/
};

unsigned char car_IO_status[][24]= {
{0x82,0xBA,0xA2,0xB6,0xAA,0xEB,0xAA,0xB6,0xA2,0xBA,0x82,0x00,0x0F,0x00,0x04,0x06,0x05,0x04,0x06,0x0C,0x00,0x08,0x0F,0x00},/*"��",0*/
{0x10,0x90,0xA8,0xA4,0xA2,0xA1,0xA2,0xA4,0xA8,0x90,0x10,0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00},/*"��",1*/
{0x0C,0xA4,0x94,0x8C,0x85,0x86,0x84,0x8C,0x94,0xA4,0x0C,0x00,0x08,0x08,0x08,0x08,0x08,0x0F,0x08,0x08,0x08,0x08,0x08,0x00},/*"��",2*/
{0x10,0xFC,0x03,0x9A,0xF6,0x92,0xBA,0x00,0xFC,0x00,0xFF,0x00,0x00,0x0F,0x00,0x04,0x07,0x02,0x02,0x00,0x01,0x08,0x0F,0x00},/*"��",3*/
{0x88,0x88,0xFF,0x48,0x12,0x94,0x90,0x9F,0x90,0x94,0xF2,0x00,0x00,0x08,0x0F,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x0F,0x00},/*"��",4*/
{0x21,0xE2,0x00,0x10,0x12,0xF2,0x12,0x12,0xF2,0x12,0x10,0x00,0x08,0x07,0x08,0x0A,0x09,0x08,0x08,0x08,0x09,0x0A,0x0B,0x00},/*"Զ",5*/
{0x20,0x22,0x24,0xE8,0x20,0x3F,0x20,0xE8,0x24,0x22,0x20,0x00,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x07,0x08,0x08,0x0E,0x00},/*"��",6*/
{0x10,0x11,0xF2,0x00,0x00,0xFE,0x12,0x12,0xF1,0x11,0x10,0x00,0x08,0x04,0x03,0x04,0x0A,0x09,0x08,0x08,0x0B,0x08,0x08,0x00},/*"��",7*/
{0x20,0x22,0x24,0xE8,0x20,0x3F,0x20,0xE8,0x24,0x22,0x20,0x00,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x07,0x08,0x08,0x0E,0x00},/*"��",8*/
{0x04,0x04,0x84,0xE4,0x5C,0x47,0x44,0x44,0x44,0xC4,0x04,0x00,0x02,0x01,0x00,0x0F,0x04,0x04,0x04,0x04,0x04,0x0F,0x00,0x00},/*"��",9*/
{0x04,0x04,0x84,0x74,0x4F,0x44,0xC4,0x44,0x44,0x44,0x04,0x00,0x04,0x02,0x09,0x08,0x08,0x08,0x0F,0x08,0x08,0x08,0x08,0x00},/*"��",10*/
{0x74,0x4F,0xF4,0x40,0x24,0xE4,0xBC,0xA7,0xA4,0xA4,0x20,0x00,0x02,0x02,0x0F,0x01,0x00,0x02,0x02,0x04,0x06,0x09,0x00,0x00},/*"ת",11*/
{0xFC,0x04,0x04,0xE6,0x25,0x24,0x24,0xE4,0x04,0x04,0xFC,0x00,0x0F,0x00,0x00,0x03,0x02,0x02,0x02,0x03,0x08,0x08,0x0F,0x00},/*"��",12*/
{0x38,0x00,0xFF,0x10,0x08,0x02,0x02,0x02,0xFE,0x02,0x02,0x00,0x08,0x06,0x01,0x02,0x0C,0x00,0x08,0x08,0x0F,0x00,0x00,0x00},/*"��",13*/
{0x18,0xD6,0x54,0xFF,0x54,0xD4,0x10,0xFC,0x00,0x00,0xFF,0x00,0x00,0x07,0x00,0x0F,0x04,0x07,0x00,0x01,0x08,0x08,0x0F,0x00},/*"��",14*/
{0x10,0xD2,0x32,0x92,0x10,0x00,0x08,0xFF,0x08,0x08,0xF8,0x00,0x03,0x02,0x02,0x02,0x03,0x08,0x06,0x01,0x08,0x08,0x07,0x00},/*"��",15*/
};

int weekday = 0;
int time_h=0,time_m=0,time_s=0;
char display_string[100];

void LCD_Clear(void)
{
	unsigned char i,j;
	
	for(i=0xB0;i<0xB7;i++)
	{
		for(j=0x10;j<=0x18;j++)
		{
			displayIcon(i,j, 0x0,screen_clear);
		}
		
	}
	
}

void displayChinese_16x16(unsigned char page,unsigned char CaddrH,unsigned char CaddrL,unsigned char arr[][24],unsigned char startWord, unsigned char endWord)
{
	unsigned char i,j;
	if(startWord == endWord)
	{
		SendCmdByte(page);
		SendCmdWord(CaddrH,CaddrL);
		for(j=0; j<12; j++)
        {
            SendDataByte(arr[startWord][j]);
        }
		SendCmdByte(page+0x01);
		SendCmdWord(CaddrH,CaddrL);
		for(j=12; j<24; j++)
		{
			SendDataByte(arr[startWord][j]);
		}
		return;
	}

	else
	{
		SendCmdByte(page);
		SendCmdWord(CaddrH,CaddrL);
		for(i=startWord; i<=endWord; i++)
		{
			for(j=0; j<12; j++)
			{
				SendDataByte(arr[i][j]);
			}

		}
		SendCmdByte(page+0x01);
		SendCmdWord(CaddrH,CaddrL);

		for(i=startWord; i<=endWord; i++)
		{
			for(j=12; j<24; j++)
			{
				SendDataByte(arr[i][j]);
			}

		}
	}
}

void showMainMenu(int time, int velocity)
{
//    printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);


    weekday = WeekYearday(calendar.w_year,calendar.w_month,calendar.w_date);


    displayIcon(0xB0,0x10, 0x0,icon_4G);
    displayIcon(0xB0,0x11, 0x0,icon_beidou);
    displayIcon(0xB0,0x12, 0x0,icon_ppl);
    displayIcon(0xB0,0x13, 0x0,icon_SD);
    displayIcon(0xB0,0x14, 0x0,icon_charged);
	displayIcon(0xB0,0x15, 0x0,icon_camera);
	
//	displayIcon(0xB0,0x14, 0x0,icon_battery);

//	displayIcon(0xB0,0x17, 0x0,icon_test);


    sprintf(display_string,"%3d",velocity);
    if(velocity > 50)
    {
		/*���ټ�ʻ*/
        displayChinese_16x16(0xB4,0x13, 0x0, over_speed, 0,1);
    }
    else
    {
        displayEmpty(0xB4,0x12, 0x0);
    }

//    sprintf(display_string,"100",3);
    if(time>1800)
    {
		/*������ʻ*/
        displayChinese_16x16(0xB2,0x10, 0x0,overTimeDriveRecord,3,6);
		time_h=time/3600;
		time_m=time%3600/60;
		time_s=time%60;
		if(time<36000 && time>1800)
		{
			sprintf(display_string,"%d",time_h);
			ShowString(0xB2,0x13, 0x0,display_string,12);
			/*ʱ*/
			displayChinese_16x16(0xB2,0x13,0x8,weekday_array,9,9);
//			sprintf(display_string,"%d",time_m);
//			ShowString(0xB2,0x14, 0x8,display_string,12);
//			/*��*/
//			displayChinese_16x16(0xB2,0x15,0x8,weekday_array,10,10);
//			sprintf(display_string,"%d",time_s);
//			ShowString(0xB2,0x16, 0x8,display_string,12);
//			/*��*/
//			displayChinese_16x16(0xB2,0x17,0x8,weekday_array,11,11);	
			sprintf(display_string,"%d",time_m);
			ShowString(0xB2,0x14, 0x5,display_string,12);
			/*��*/
			displayChinese_16x16(0xB2,0x15,0x2,weekday_array,10,10);
			sprintf(display_string,"%d",time_s);
			ShowString(0xB2,0x16, 0x0,display_string,12);
			/*��*/
			displayChinese_16x16(0xB2,0x16,0xe,weekday_array,11,11);			
		}
		else if(time>36000)
		{
			sprintf(display_string,"%d",time_h);
			ShowString(0xB2,0x13, 0x0,display_string,12);
			/*ʱ*/
			displayChinese_16x16(0xB2,0x13,0xd,weekday_array,9,9);
			sprintf(display_string,"%d",time_m);
			ShowString(0xB2,0x14, 0xa,display_string,12);
			/*��*/
			displayChinese_16x16(0xB2,0x15,0x8,weekday_array,10,10);
			sprintf(display_string,"%d",time_s);
			ShowString(0xB2,0x16, 0x5,display_string,12);
			/*��*/
			displayChinese_16x16(0xB2,0x17,0x3,weekday_array,11,11);
		}

    }
    else
    {
        displayEmpty(0xB2,0x10, 0x0);
    }
	
    displayWeekday(0xB4,0x10,0x0,weekday);
//	displayChinese_16x16(0xB4,0x13,0x0,abnormal_speed,0,4);
	sprintf(display_string,"%d",velocity);
	if(velocity<100)
	{
		ShowString(0xB4,0x15, 0x8,display_string,12);
	}
    else
	{
		ShowString(0xB4,0x15, 0x0,display_string,12);
	}
    sprintf(display_string,"km/h");
    ShowString(0xB4,0x16, 0x04,display_string,12);
    sprintf(display_string,"%04d-%02d-%02d,%02d:%02d:%02d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
    ShowString(0xB6,0x10, 0x00,display_string,12);
}


int WeekYearday(int years, int months, int days)
{
    int WeekDay=-1;
    if(1==months || 2==months)
    {
        months+=12;
        years--;
    }
    WeekDay=(days+1+2*months+3*(months+1)/5+years+years/4-years/100+years/400)%7;

    return WeekDay;
}

void displayWeekday(unsigned char page,unsigned char CaddrH,unsigned char CaddrL,int weekday)
{
    unsigned char i,j;

    SendCmdByte(page);
    SendCmdWord(CaddrH,CaddrL);
    for(i=0; i<2; i++)
    {
        for(j=0; j<12; j++)
        {
            SendDataByte(weekday_array[i][j]);
        }

    }
    for(j=0; j<12; j++)
    {
        SendDataByte(weekday_array[weekday+1][j]);
    }
    SendCmdByte(page+0x01);
    SendCmdWord(CaddrH,CaddrL);

    for(i=0; i<2; i++)
    {
        for(j=12; j<24; j++)
        {
            SendDataByte(weekday_array[i][j]);
        }

    }
    for(j=12; j<24; j++)
    {
        SendDataByte(weekday_array[weekday+1][j]);
    }

}

////void displayCarDriverInfo(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
////{
////    unsigned char i,j;

////    SendCmdByte(page);
////    SendCmdWord(CaddrH,CaddrL);
////    for(i=0; i<8; i++)
////    {
////        for(j=0; j<12; j++)
////        {
////            SendDataByte(vehicle_driver_info[i][j]);
////        }

////    }
////    SendCmdByte(page+0x01);
////    SendCmdWord(CaddrH,CaddrL);

////    for(i=0; i<8; i++)
////    {
////        for(j=12; j<24; j++)
////        {
////            SendDataByte(vehicle_driver_info[i][j]);
////        }

////    }

////}

//void displayDriverInfo(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//	unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=3; i<8; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=3; i<8; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }
//}

//void displayCarInfo(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<2; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }
//	for(i=6; i<8; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<2; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }
//	for(i=6; i<8; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(vehicle_driver_info[i][j]);
//        }

//    }

//}


//void displayCarPlateInfo(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//	unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<5; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }
//	for(i=7; i<9; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<5; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }
//	for(i=7; i<9; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }
//	
//}


//void displayCarPlateNumber(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<7; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<7; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(Chinese_car_plate[i][j]);
//        }

//    }

//}

//void displayNoOverTimeRecord(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<7; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(overTimeDriveRecord[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<7; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(overTimeDriveRecord[i][j]);
//        }

//    }

//}

//void displayOverTimeRecord(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=1; i<7; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(overTimeDriveRecord[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=1; i<7; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(overTimeDriveRecord[i][j]);
//        }

//    }

//}

void displayEmpty(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
{
    unsigned char i,j;

    SendCmdByte(page);
    SendCmdWord(CaddrH,CaddrL);
    for(i=0; i<4; i++)
    {
        for(j=0; j<12; j++)
        {
            SendDataByte(empty[i][j]);
        }

    }
    SendCmdByte(page+0x01);
    SendCmdWord(CaddrH,CaddrL);

    for(i=0; i<4; i++)
    {
        for(j=12; j<24; j++)
        {
            SendDataByte(empty[i][j]);
        }

    }
}

//void displayChineseBreak(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<4; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(need_break[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<4; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(need_break[i][j]);
//        }

//    }

//}


//void displayChineseSpeed(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
//{
//    unsigned char i,j;

//    SendCmdByte(page);
//    SendCmdWord(CaddrH,CaddrL);
//    for(i=0; i<4; i++)
//    {
//        for(j=0; j<12; j++)
//        {
//            SendDataByte(over_speed[i][j]);
//        }

//    }
//    SendCmdByte(page+0x01);
//    SendCmdWord(CaddrH,CaddrL);

//    for(i=0; i<4; i++)
//    {
//        for(j=12; j<24; j++)
//        {
//            SendDataByte(over_speed[i][j]);
//        }

//    }

//}


void displayIcon(unsigned char page,unsigned char CaddrH,unsigned char CaddrL, unsigned char arr[2][16])
{
    unsigned char j;

    SendCmdByte(page);
    SendCmdWord(CaddrH,CaddrL);

    for(j=0; j<16; j++)
    {
        SendDataByte(arr[0][j]);
    }


    SendCmdByte(page+0x01);
    SendCmdWord(CaddrH,CaddrL);

    for(j=0; j<16; j++)
    {
        SendDataByte(arr[1][j]);
    }
}









