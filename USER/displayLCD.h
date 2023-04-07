
#include "sys.h"	    



void showMainMenu(void);




void displayChineseSpeed(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayChineseBreak(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayWeekday(unsigned char page,unsigned char CaddrH,unsigned char CaddrL,int weekday);

void displayIcon(unsigned char page,unsigned char CaddrH,unsigned char CaddrL, unsigned char arr[2][16]);

void displayIcon4G(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayBeidou(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
void displayPPL(unsigned char page,unsigned char CaddrH,unsigned char CaddrL);
int WeekYearday(int years, int months, int days);


















