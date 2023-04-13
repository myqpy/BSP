
#include "menu.h"
#include "displayLCD.h"
#include "gpio.h"
#include "delay.h"
#include "ST7567a.h"
#include "terminal_parameter.h"
#include <stdio.h>


//struct struct_rk_info *rk_info;
int page = 0;
int page_min = 0;
int page_max = 3;
int page_status = 0;

unsigned char page1_row_min = 0xB0;
unsigned char page1_row = 0xB0;
unsigned char page1_row_max = 0xB2;

unsigned char page2_row_min = 0xB0;
unsigned char page2_row = 0xB0;
unsigned char page2_row_max = 0xB2;

unsigned char page3_row_min = 0xB0;
unsigned char page3_row = 0xB0;
unsigned char page3_row_max = 0xB4;

//unsigned char page3_subPage_row_min = 0xB0;
//unsigned char page3_subPage_row = 0xB0;
//unsigned char page3_subPage_row_max = 0xB2;

u8 key_text=0;
char printString[100];
//void key_process(void)}
//{
//
//}

void MENU_processing(void)
{
    key_text=KEY_Scan(0);		//�õ���ֵ
    if(page<=0)
    {
        page_status=page;
        switch(key_text)
        {
//        case KEY_UpArrow_PRES:
//            hang--;
//            if(hang>=hang_max) {
//                hang=hang_min;
//            }
//            if(hang<=hang_min) {
//                hang=hang_max;
//            }
//            LCD_Clear();
//            break;
//        case KEY_DownArrow_PRES:
//            hang++;
//            if(hang>=hang_max) {
//                hang=hang_min;

//            }
//            if(hang<=hang_min) {
//                hang=hang_max;

//            }
//            LCD_Clear();
//            break;
        case KEY_menu_PRES:
            page=page+1;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
//        case KEY_confirmed_PRES:
//            page=page+1;
//            if(page>=page_max) {
//                page=page_max;
//            }
//            if(page<=page_min) {
//                page=page_min;
//            }
//            LCD_Clear();
//            break;

        }

        showMainMenu(0,0);

        if(page_status!=page)
        {
            LCD_Clear();
        }
    }
    else if(page==1)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page1_row-=2;
		
            if(page1_row>=page1_row_max) {
                page1_row=page1_row_max;
            }
            if(page1_row<=page1_row_min) {
                page1_row=page1_row_min;
            }
            LCD_Clear();
            break;
        case KEY_DownArrow_PRES:
            page1_row+=2;
            if(page1_row>=page1_row_max) {
                page1_row=page1_row_max;

            }
            if(page1_row<=page1_row_min) {
                page1_row=page1_row_min;

            }
            LCD_Clear();
            break;
        case KEY_menu_PRES:
            page--;
			page1_row = 0xB0;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        case KEY_confirmed_PRES:
            page++;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        }
        /*��������ʻԱ��Ϣ*/
        displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,7);
        /*��ʱ��ʻ��¼*/
        displayChinese_16x16(0xB2,0x12,0x0,overTimeDriveRecord,1,6);
        sprintf(printString,"-->");
        ShowString(page1_row,0x10, 0x0,printString,12);

        if(page_status!=page)
        {
            LCD_Clear();
        }
    }
    else if(page==2)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page2_row-=2;
            if(page2_row>=page2_row_max) {
                page2_row=page2_row_max;
            }
            if(page2_row<=page2_row_min) {
                page2_row=page2_row_min;
            }
            LCD_Clear();
            break;
        case KEY_DownArrow_PRES:
            page2_row+=2;
            if(page2_row>=page2_row_max) {
                page2_row=page2_row_max;

            }
            if(page2_row<=page2_row_min) {
                page2_row=page2_row_min;

            }
            LCD_Clear();
            break;
        case KEY_menu_PRES:
            page--;
			page2_row = 0xB0;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        case KEY_confirmed_PRES:
            page++;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        }
        if(page1_row==0xB0)
        {
            /*������Ϣ*/
            displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,1);
            displayChinese_16x16(0xB0,0x14,0x0,vehicle_driver_info,6,7);

            /*��ʻԱ��Ϣ*/
            displayChinese_16x16(0xB2,0x12,0x0,vehicle_driver_info,3,7);
            sprintf(printString,"-->");
            ShowString(page2_row,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2)
        {
            /*�޳�ʱ��ʻ��¼*/
            displayChinese_16x16(0xB3,0x12,0x0,overTimeDriveRecord,0,6);
        }


        if(page_status!=page)
        {
            LCD_Clear();
        }
    }

    else if(page==3)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page3_row-=2;
            if(page3_row>=page3_row_max) {
                page3_row=page3_row_max;
            }
            if(page3_row<=page3_row_min) {
                page3_row=page3_row_min;
            }
            LCD_Clear();
            break;
        case KEY_DownArrow_PRES:
            page3_row+=2;
            if(page3_row>=page3_row_max) {
                page3_row=page3_row_max;

            }
            if(page3_row<=page3_row_min) {
                page3_row=page3_row_min;

            }
            LCD_Clear();
            break;
        case KEY_menu_PRES:
            page--;
			page3_row = 0xB0;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        case KEY_confirmed_PRES:
            page++;
			page3_row = 0xB0;
            if(page>=page_max) {
                page=page_max;
            }
            if(page<=page_min) {
                page=page_min;
            }
            LCD_Clear();
            break;
        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB0)
        {
            /*���������Ʒ���*/
            displayChinese_16x16(0xB0,0x10,0x0,Chinese_car_plate,0,6);
            /*������������ɫ*/
            displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,1,1);
            displayChinese_16x16(0xB0,0x17,0x0,car_plate_color,4,4);
            /*���������ƺ���*/
            displayChinese_16x16(0xB2,0x10,0x0,Chinese_car_plate,0,4);
            displayChinese_16x16(0xB2,0x14,0x0,Chinese_car_plate,10,11);
			
            displayChinese_16x16(0xB4,0x10,0x0,car_plate_province,10,10);
            
			sprintf(printString,"A88888");
            ShowString(0xB4,0x11, 0x0,printString,12);
            displayChinese_16x16(0xB6,0x10,0x0,pulseRatio,0,3);
            sprintf(printString,"6000");
            ShowString(0xB6,0x14, 0x0,printString,12);

//			sprintf(printString,"-->");
//			ShowString(page2_row,0x10, 0x0,printString,12);
        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB2)
        {
//			LCD_Clear();
            /*���*/
            displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,0,1);
            /*�յ�*/
            displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,2,2);
            displayChinese_16x16(0xB2,0x11,0x0,car_IO_status,4,4);
            /*����*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,3,4);
            /*Զ���*/
            displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,5,6);
            displayChinese_16x16(0xB6,0x12,0x0,car_IO_status,13,13);

        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB4)
        {
//			LCD_Clear();
            /*�����*/
            displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,7,8);
            displayChinese_16x16(0xB0,0x12,0x0,car_IO_status,13,13);
            /*��ת���*/
            displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,9,9);
            displayChinese_16x16(0xB2,0x11,0x0,car_IO_status,11,12);
            displayChinese_16x16(0xB2,0x13,0x0,car_IO_status,13,13);
            /*��ת���*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,10,12);
            displayChinese_16x16(0xB4,0x13,0x0,car_IO_status,13,13);
            /*�ƶ�*/
            displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,14,15);
        }
		if(page1_row==0xB0 && page2_row==0xB0 && page3_row>0xB4)
		{
			page3_row = 0xB0;
		}
        if(page1_row==0xB0 && page2_row==0xB2)
        {
            /*��������ʻ֤����*/
            displayChinese_16x16(0xB0,0x10,0x0,Chinese_car_plate,0,2);
            displayChinese_16x16(0xB0,0x13,0x0,Chinese_car_plate,7,11);
            sprintf(printString,"410105199607150035");
            ShowString(0xB2,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2)
        {
            page--;
        }


        if(page_status!=page)
        {
            LCD_Clear();
        }
    }
}
