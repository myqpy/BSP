#include "displayLCD.h"
#include "usart.h"
#include "ST7567a.h"
#include "rtc.h" 	

int weekday = 0;
u8 display_string[100];

void showMainMenu(void)
{
	printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);


			weekday = WeekYearday(calendar.w_year,calendar.w_month,calendar.w_date);
			
			
			displayIcon(0xB0,0x10, 0x0,icon_4G);
			displayIcon(0xB0,0x11, 0x0,icon_beidou);
			displayIcon(0xB0,0x12, 0x0,icon_ppl);
			displayIcon(0xB0,0x13, 0x0,icon_SD);
			displayIcon(0xB0,0x14, 0x0,icon_charged);
//			displayIcon(0xB0,0x14, 0x0,icon_battery);
			
//			displayIcon(0xB0,0x17, 0x0,icon_test);
			
			
			
			
            displayChineseBreak(0xB2,0x10, 0x0);
			displayWeekday(0xB4,0x10,0x0,weekday);
//            displayChineseSpeed(0xB4,0x10, 0x0);
//			sprintf(display_string,"%3d",velocity);
            sprintf(display_string,"100",3);
            ShowString(0xB2,0x13, 0x0c,display_string,24);
            sprintf(display_string,"km/h");
            ShowString(0xB3,0x16, 0x04,display_string,16);
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

void displayChineseBreak(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
{
    unsigned char i,j;

    SendCmdByte(page);
    SendCmdWord(CaddrH,CaddrL);
    for(i=0; i<4; i++)
    {
        for(j=0; j<12; j++)
        {
            SendDataByte(need_break[i][j]);
        }

    }
    SendCmdByte(page+0x01);
    SendCmdWord(CaddrH,CaddrL);

    for(i=0; i<4; i++)
    {
        for(j=12; j<24; j++)
        {
            SendDataByte(need_break[i][j]);
        }

    }

}



void displayChineseSpeed(unsigned char page,unsigned char CaddrH,unsigned char CaddrL)
{
    unsigned char i,j;

    SendCmdByte(page);
    SendCmdWord(CaddrH,CaddrL);
    for(i=0; i<4; i++)
    {
        for(j=0; j<12; j++)
        {
            SendDataByte(over_speed[i][j]);
        }

    }
    SendCmdByte(page+0x01);
    SendCmdWord(CaddrH,CaddrL);

    for(i=0; i<4; i++)
    {
        for(j=12; j<24; j++)
        {
            SendDataByte(over_speed[i][j]);
        }

    }

}


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









