
#include "menu.h"
#include "displayLCD.h"
#include "gpio.h"
#include "delay.h"
#include "ST7567a.h"
#include "terminal_parameter.h"
#include "printer.h"
#include <stdio.h>


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
u16 confirmed_pressed=0, up_pressed=0, down_pressed=0;
u8 key_text=0;
char printString[100];

//void key_process(void)}
//{
//
//}

void MENU_processing(struct struct_rk_info *rk_info, int time_second,int velocity)
{

    key_text=KEY_Scan(1);		//得到键值

    if(key_text!=0)time_1ms = 0;
    if(time_1ms>=10000)
    {
        TIM_Cmd(TIM5,DISABLE);
        LCD_Clear();
        page=0;
        time_1ms = 0;
        confirmed_pressed  = 0;
		up_pressed=0;
		down_pressed=0;
    }

    if(page<=0)
    {
        page_status=page;
        switch(key_text)
        {
        case KEY_UpArrow_PRES:
			printf("up_pressed = %d\r\n",up_pressed);
            TIM_Cmd(TIM5,DISABLE);
            TIM_Cmd(TIM5,ENABLE);
			if(up_pressed >= 3000)
            {
                printf("printing!!!!!!!!! \r\n");
//				printer_info_init();
                confirmed_pressed  = 0;
                TIM_Cmd(TIM5,DISABLE);
            }
            break;
        case KEY_DownArrow_PRES:
			printf("down_pressed = %d\r\n ",down_pressed);
            TIM_Cmd(TIM5,DISABLE);
            TIM_Cmd(TIM5,ENABLE);
			if(down_pressed >= 3000)
            {
                printf("printing!!!!!!!!! \r\n");
//				printer_info_init();
                confirmed_pressed  = 0;
                TIM_Cmd(TIM5,DISABLE);
            }
            break;
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
//				printer_info_init();
                confirmed_pressed  = 0;
                TIM_Cmd(TIM5,DISABLE);
            }

            break;

        }
		
		if(up_pressed>=3000&&down_pressed>=3000)
		{
			printf("SOS!!!!!!!!! \r\n");
		}
        showMainMenu(time_second,velocity, rk_info);

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
        /*车辆及驾驶员信息*/
        displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,7);
        /*超时驾驶记录*/
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
            /*车辆信息*/
            displayChinese_16x16(0xB0,0x12,0x0,vehicle_driver_info,0,1);
            displayChinese_16x16(0xB0,0x13,0x8,vehicle_driver_info,6,7);

            /*驾驶员信息*/
            displayChinese_16x16(0xB2,0x12,0x0,vehicle_driver_info,3,7);
            sprintf(printString,"-->");
            ShowString(page2_row,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2)
        {
            /*无超时驾驶记录*/
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
            /*机动车号牌分类*/
            displayChinese_16x16(0xB0,0x10,0x0,Chinese_car_plate,0,6);
            /*机动车号牌颜色*/
            displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,1,1);
            displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
            /*机动车号牌号码*/
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
            /*离合*/
            displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,0,1);
            /*空挡*/
            displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,2,2);
            displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,4,4);
            /*倒挡*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,3,4);
            /*远光灯*/
            displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,5,6);
            displayChinese_16x16(0xB6,0x11,0x8,car_IO_status,13,13);

        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB4)
        {
//			LCD_Clear();
            /*近光灯*/
            displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,7,8);
            displayChinese_16x16(0xB0,0x11,0x8,car_IO_status,13,13);
            /*右转向灯*/
            displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,9,9);
            displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,11,12);
            displayChinese_16x16(0xB2,0x12,0x4,car_IO_status,13,13);
            /*左转向灯*/
            displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,10,12);
            displayChinese_16x16(0xB4,0x12,0x5,car_IO_status,13,13);
            /*制动*/
            displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,14,15);
        }
        if(page1_row==0xB0 && page2_row==0xB0 && page3_row>0xB4)
        {
            page3_row = 0xB0;
        }
        if(page1_row==0xB0 && page2_row==0xB2)
        {
            /*机动车驾驶证号码*/
            displayChinese_16x16(0xB0,0x10,0x0,Chinese_car_plate,0,2);
            displayChinese_16x16(0xB0,0x12,0x6,Chinese_car_plate,7,11);
            sprintf(printString,"410105199607150035");
            ShowString(0xB2,0x10, 0x0,printString,12);
        }

        if(page1_row==0xB2) page--;

        if(page_status!=page)LCD_Clear();

    }
}


//定时器5中断服务程序
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
        time_1ms++;
        if(page==0)
        {
            if(key_text == KEY_confirmed_PRES)
            {
                confirmed_pressed++;
            }
            else if(key_text == KEY_UpArrow_PRES)
            {
                up_pressed++;
            }
            else if(key_text == KEY_DownArrow_PRES)
            {
                down_pressed++;
            }
        }
    }
}

