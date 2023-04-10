
#include "sys.h"	    



void showMainMenu(int time, int velocity);

extern unsigned char need_break[][24];
extern unsigned char over_speed[][24];
extern unsigned char weekday_array[][24];
extern unsigned char icon_test[][16];
extern unsigned char icon_4G[][16];
extern unsigned char icon_beidou[][16];
extern unsigned char icon_ppl[][16];
extern unsigned char icon_SD[][16];
extern unsigned char icon_charged[][16];
extern unsigned char icon_battery[][16];

void displayEmpty(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayChineseSpeed(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayChineseBreak(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayWeekday(unsigned char page,unsigned char CaddrH,unsigned char CaddrL,int weekday);

void displayIcon(unsigned char page,unsigned char CaddrH,unsigned char CaddrL, unsigned char arr[2][16]);

void displayIcon4G(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayBeidou(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayPPL(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
int WeekYearday(int years, int months, int days);


















