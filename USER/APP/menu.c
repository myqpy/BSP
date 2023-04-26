#include "printercmd.h"
#include "menu.h"
#include "displayLCD.h"
#include "gpio.h"
#include "delay.h"
#include "ST7567a.h"

#include "printer.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"


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
u16 time_1ms=0;
u16 confirmed_pressed=0;
u16 up_down_pressed=0;
u8 key_text=0;
char printString[100];
extern u8 printer_cmd[200];
ARM_vehicle_info rk_vehicle_info;

void MENU_processing(ARM_selfCheck_info *rk_selfCheck_info, int time_second,int velocity)
{

    key_text=KEY_Scan(1);		//�õ���ֵ

    if(key_text!=0)time_1ms = 0;
    if(time_1ms>=10000)
    {
        TIM_Cmd(TIM5,DISABLE);
        LCD_Clear();
        page=0;
        time_1ms = 0;
        confirmed_pressed  = 0;
		up_down_pressed=0;
    }

    if(page<=0)
    {
        page_status=page;
        switch(key_text)
        {
		case KEY_up_down_PRES:
			TIM_Cmd(TIM5,DISABLE);
            TIM_Cmd(TIM5,ENABLE);
			printf("up_down_pressed = %d\r\n",up_down_pressed);
			if(up_down_pressed>=3000)
			{
				printf("SOS!!!!!!!!! \r\n");
				up_down_pressed = 0;
				TIM_Cmd(TIM5,DISABLE);
			}
			break;
//        case KEY_UpArrow_PRES:
//			printf("up_pressed = %d\r\n",up_pressed);
//            TIM_Cmd(TIM5,DISABLE);
//            TIM_Cmd(TIM5,ENABLE);
//			if(up_pressed >= 3000)
//            {
//                printf("printing!!!!!!!!! \r\n");
//				printer_info_init();
//                confirmed_pressed  = 0;
//                TIM_Cmd(TIM5,DISABLE);
//            }
//            break;
//        case KEY_DownArrow_PRES:
//			printf("down_pressed = %d\r\n ",down_pressed);
//            TIM_Cmd(TIM5,DISABLE);
//            TIM_Cmd(TIM5,ENABLE);
//			if(down_pressed >= 3000)
//            {
//                printf("printing!!!!!!!!! \r\n");
////				printer_info_init();
//                confirmed_pressed  = 0;
//                TIM_Cmd(TIM5,DISABLE);
//            }
//            break;
        case KEY_menu_PRES:
            TIM_Cmd(TIM5,DISABLE);
            TIM_Cmd(TIM5,ENABLE);
            page=page+1;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;
        case KEY_confirmed_PRES:
            TIM_Cmd(TIM5,DISABLE);
            TIM_Cmd(TIM5,ENABLE);

            printf("confirmed_pressed = %d \r\n",confirmed_pressed);

            if(confirmed_pressed >= 3000)
            {
                printf("printing!!!!!!!!! \r\n");
//				memset(rk_vehicle_info->car_plate_num,0,sizeof(*rk_vehicle_info->car_plate_num));
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"���������ƺ��룺", sizeof("���������ƺ��룺"));
				Printer_printString(printer_cmd);
				
				memset(rk_vehicle_info.car_plate_num,0,16);
				memcpy(rk_vehicle_info.car_plate_num,"ԥA88888",8);
				Printer_printString(rk_vehicle_info.car_plate_num);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"���������Ʒ��ࣺ", sizeof("���������Ʒ��ࣺ"));
				Printer_printString(printer_cmd);
				
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"��ɫ", sizeof("��ɫ"));
				Printer_printString(printer_cmd);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"��������ʻ֤���룺", sizeof("��������ʻ֤���룺"));
				Printer_printString(printer_cmd);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"410105199607150035", sizeof("410105199607150035"));
				Printer_printString(printer_cmd);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"�ٶ�״̬��", sizeof("�ٶ�״̬��"));
				Printer_printString(printer_cmd);
				
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"����", sizeof("����"));
				Printer_printString(printer_cmd);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"��ӡʱ��", sizeof("��ӡʱ��"));
				Printer_printString(printer_cmd);
				
				memset(printer_cmd ,0,sizeof(printer_cmd));
				sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour,calendar.min,calendar.sec);	
				Printer_printString(printer_cmd);
				
				print_empty_line();
				memset(printer_cmd ,0,sizeof(printer_cmd));
				memcpy(printer_cmd,"�����������ڳ�ʱ��ʻ��¼��", sizeof("�����������ڳ�ʱ��ʻ��¼��"));
				Printer_printString(printer_cmd);
				
				print_empty_line();
				print_empty_line();
				print_empty_line();
				print_empty_line();
                confirmed_pressed  = 0;
                TIM_Cmd(TIM5,DISABLE);
            }

            break;

        }
		

        showMainMenu(time_second,velocity, rk_selfCheck_info);

        if(page_status!=page) LCD_Clear();
    }
    else if(page==1)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page1_row-=2;
            if(page1_row>=page1_row_max) page1_row=page1_row_max;
            if(page1_row<=page1_row_min) page1_row=page1_row_min;
            LCD_Clear();
            break;

        case KEY_DownArrow_PRES:
            page1_row+=2;
            if(page1_row>=page1_row_max) page1_row=page1_row_max;
            if(page1_row<=page1_row_min) page1_row=page1_row_min;
            LCD_Clear();
            break;

        case KEY_menu_PRES:
            page--;
            page1_row = 0xB0;
            if(page>=page_max) page=page_max;
            if(page<=page_min)page=page_min;
            LCD_Clear();
            break;

        case KEY_confirmed_PRES:
            page++;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;
        }
        /*��������ʻԱ��Ϣ*/
        displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,7);
        /*��ʱ��ʻ��¼*/
        displayChinese_16x16(0xB2,0x12,0x0,overTimeDriveRecord,1,2);
        displayChinese_16x16(0xB2,0x13,0x8,overTimeDriveRecord,5,8);
        sprintf(printString,"-->");
        ShowString(page1_row,0x10, 0x0,printString,12);

        if(page_status!=page) LCD_Clear();

    }
    else if(page==2)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page2_row-=2;
            if(page2_row>=page2_row_max) page2_row=page2_row_max;
            if(page2_row<=page2_row_min) page2_row=page2_row_min;
            LCD_Clear();
            break;

        case KEY_DownArrow_PRES:
            page2_row+=2;
            if(page2_row>=page2_row_max) page2_row=page2_row_max;
            if(page2_row<=page2_row_min) page2_row=page2_row_min;
            LCD_Clear();
            break;

        case KEY_menu_PRES:
            page--;
            page2_row = 0xB0;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;

        case KEY_confirmed_PRES:
            page++;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;
        }
        if(page1_row==0xB0)
        {
            /*������Ϣ*/
            displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,1);
            displayChinese_16x16(0xB0,0x13,0x8,vehicle_driver_info,6,7);

            /*��ʻԱ��Ϣ*/
            displayChinese_16x16(0xB2,0x12,0x0,vehicle_driver_info,3,7);
            sprintf(printString,"-->");
            ShowString(page2_row,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2)
        {
            /*�޳�ʱ��ʻ��¼*/
            displayChinese_16x16(0xB3,0x12,0x0,overTimeDriveRecord,0,2);
            displayChinese_16x16(0xB3,0x14,0x5,overTimeDriveRecord,5,8);
        }

        if(page_status!=page) LCD_Clear();
    }

    else if(page==3)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
            page3_row-=2;
            if(page3_row>=page3_row_max) page3_row=page3_row_max;
            if(page3_row<=page3_row_min) page3_row=page3_row_min;
            LCD_Clear();
            break;

        case KEY_DownArrow_PRES:
            page3_row+=2;
            if(page3_row>=page3_row_max) page3_row=page3_row_max;
            if(page3_row<=page3_row_min) page3_row=page3_row_min;
            LCD_Clear();
            break;

        case KEY_menu_PRES:
            page--;
            page3_row = 0xB0;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;

        case KEY_confirmed_PRES:
            page++;
            page3_row = 0xB0;
            if(page>=page_max) page=page_max;
            if(page<=page_min) page=page_min;
            LCD_Clear();
            break;
        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB0)
        {
            /*���������Ʒ���*/
            displayChinese_16x16(0xB0,0x10,0x0,Chinese_car_plate,0,6);
            /*������������ɫ*/
            displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,1,1);
            displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
            /*���������ƺ���*/
            displayChinese_16x16(0xB2,0x10,0x0,Chinese_car_plate,0,4);
            displayChinese_16x16(0xB2,0x13,0xd,Chinese_car_plate,10,11);

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
            displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,4,4);
            /*����*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,3,4);
            /*Զ���*/
            displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,5,6);
            displayChinese_16x16(0xB6,0x11,0x8,car_IO_status,13,13);

        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB4)
        {
//			LCD_Clear();
            /*�����*/
            displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,7,8);
            displayChinese_16x16(0xB0,0x11,0x8,car_IO_status,13,13);
            /*��ת���*/
            displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,9,9);
            displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,11,12);
            displayChinese_16x16(0xB2,0x12,0x4,car_IO_status,13,13);
            /*��ת���*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,10,12);
            displayChinese_16x16(0xB4,0x12,0x5,car_IO_status,13,13);
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
            displayChinese_16x16(0xB0,0x12,0x6,Chinese_car_plate,7,11);
            sprintf(printString,"410105199607150035");
            ShowString(0xB2,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2) page--;

        if(page_status!=page)LCD_Clear();

    }
}


//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
        time_1ms++;
        if(page==0)
        {
            if(key_text == KEY_confirmed_PRES)
            {
                confirmed_pressed++;
            }
            if(key_text == KEY_up_down_PRES)
            {
                up_down_pressed++;
            }
        }
    }
}
