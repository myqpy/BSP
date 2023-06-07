#include "printercmd.h"
#include "packager.h"
#include "menu.h"
#include "displayLCD.h"
#include "gpio.h"
#include "delay.h"
#include "ST7567a.h"
#include "client_manager.h"
#include "printer.h"
#include <string.h>
#include "rtc.h"
#include "24cxx.h"


int page = 0;
int page_min = 0;
int page_max = 3;
int page_status = 0;

unsigned char page1_row_min = 0xB0;
unsigned char page1_row = 0xB0;
unsigned char page1_row_max = 0xB4;

unsigned char page2_row_min = 0xB0;
unsigned char page2_row = 0xB0;
unsigned char page2_row_max = 0xB2;

unsigned char page3_row_min = 0xB0;
unsigned char page3_row = 0xB0;
unsigned char page3_row_max = 0xB4;

unsigned char i,j;
u16 time_1ms=0;
u16 confirmed_pressed=0;
u16 up_down_pressed=0;
u8 key_text=0;
//u8 OTnumber=1;
u8 OTnumberNow = 0;
char printString[100];
char carPlateString[20];
unsigned char plateHead;
uint8_t noOTflag=0, uparrowLCDclear=0;


void MENU_processing(MCU_Parameters *para)
{

    key_text=KEY_Scan(1);		//得到键值

	/*********按键后重新计时*********/
    if(key_text!=0)time_1ms = 0;	

	/*********当计时大于10秒时重置*********/
    if(time_1ms>=10000)
    {
		TIM_Cmd(TIM5,DISABLE);
		LCD_Clear();
		/*********回到主界面*********/
		page=0;
		/*********重新计时*********/
		time_1ms = 0;
		confirmed_pressed  = 0;
		up_down_pressed=0;
		para->OT_info.print_flag = 0;
		if(noOTflag!=0) noOTflag=0;
		if(OTnumberNow!=0) OTnumberNow=0;
		if(para->OT_info.OTpageNum_Show!=1) para->OT_info.OTpageNum_Show=1;
    }

	/*********主菜单*********/
    if(page<=0)
    {
		page0_process(para);	
		showMainMenu(para);
		if(page_status!=page) LCD_Clear();
    }
		
    else if(page==1)
    {		
	/*********一级目录*********/
		/*
		车辆及驾驶员信息
		超时驾驶记录
		更改载货状态
		*/
		page1_process(para);	
    }
    else if(page==2)
    {
	/*********二级目录*********/
		page2_process(para);	
    }

    else if(page==3)
    {
	/*********三级目录*********/
		page3_process(para);	
    }
}

void page0_process(MCU_Parameters *para)
{
	page_status=page;
	switch(key_text)
	{
	case KEY_up_down_PRES:
//		TIM_Cmd(TIM5,DISABLE);
		TIM_Cmd(TIM5,ENABLE);
		printf("up_down_pressed = %d\r\n",up_down_pressed);
		if(up_down_pressed>=3000)
		{
			printf("SOS!!!!!!!!! \r\n");
			delay_ms(1);
			update_status(kSOS, 1);
			sendMessage(kMCUAlarmReport);
			up_down_pressed = 0;
			TIM_Cmd(TIM5,DISABLE);
		}
		break;
	case KEY_menu_PRES:
		TIM_Cmd(TIM5,ENABLE);
		page=page+1;
		if(page>=page_max) page=page_max;
		if(page<=page_min) page=page_min;
		LCD_Clear();
		break;
	case KEY_confirmed_PRES:
		TIM_Cmd(TIM5,ENABLE);

		printf("confirmed_pressed = %d \r\n",confirmed_pressed);

		if(confirmed_pressed >= 3000)
		{
			printf("printing!!!!!!!!! \r\n");
			
			para->OT_info.OTpageNum_print = 1;
			para->packager.OTpageNum = para->OT_info.OTpageNum_print;
			para->OT_info.print_flag = 1;
			sendMessage(kAcquireOTReport); 
			confirmed_pressed  = 0;
			TIM_Cmd(TIM5,DISABLE);
		}
		break;
//	case KEY_UpArrow_PRES:
//		printf("up_pressed = %d\r\n",up_pressed);
//		TIM_Cmd(TIM5,DISABLE);
//		TIM_Cmd(TIM5,ENABLE);
//		if(up_pressed >= 3000)
//		{
//			printf("printing!!!!!!!!! \r\n");
//			printer_info_init();
//			confirmed_pressed  = 0;
//			TIM_Cmd(TIM5,DISABLE);
//		}
//		break;
//	case KEY_DownArrow_PRES:
//		printf("down_pressed = %d\r\n ",down_pressed);
//		TIM_Cmd(TIM5,DISABLE);
//		TIM_Cmd(TIM5,ENABLE);
//		if(down_pressed >= 3000)
//		{
//			printf("printing!!!!!!!!! \r\n");
////				printer_info_init();
//			confirmed_pressed  = 0;
//			TIM_Cmd(TIM5,DISABLE);
//		}
//		break;
	}
	if(OTnumberNow!=0) OTnumberNow=0;
	if(para->OT_info.OTpageNum_Show!=1) para->OT_info.OTpageNum_Show=1;
	if(noOTflag!=0) noOTflag=0;
}

void page1_process(MCU_Parameters *para)
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
	/*更改载货状态*/
	displayChinese_16x16(0xB4,0x12,0x0,changeLoadingStatus,0,5);
	sprintf(printString,"-->");
	
	ShowString(page1_row,0x10, 0x0,printString,12);
	

	if(page_status!=page) LCD_Clear();
}

void page2_process(MCU_Parameters *para)
{
	page_status=page;
	switch(key_text)
	{
	case KEY_UpArrow_PRES:
		page2_row-=2;
		if(page2_row>=page2_row_max) page2_row=page2_row_max;
		if(page2_row<=page2_row_min) page2_row=page2_row_min;
		if(page1_row==0xB2)
		{
			para->OT_info.OTpageNum_Show --;
			if(para->OT_info.OTpageNum_Show <= 1) para->OT_info.OTpageNum_Show = 1;
		}
		if(noOTflag!=0) noOTflag=0;
		LCD_Clear();
		break;

	case KEY_DownArrow_PRES:
		page2_row+=2;
		if(page2_row>=page2_row_max) page2_row=page2_row_max;
		if(page2_row<=page2_row_min) page2_row=page2_row_min;
		if(page1_row==0xB2)
		{
			if(para->parse.OvertimeDriveRecord.OTnumber!=0xFF) para->OT_info.OTpageNum_Show ++;
			if(para->OT_info.OTpageNum_Show >= 100) para->OT_info.OTpageNum_Show = 100;
		}
		if(noOTflag!=0) noOTflag=0;
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
		if(OTnumberNow!= para->OT_info.OTpageNum_Show)
		{
			para->packager.OTpageNum = para->OT_info.OTpageNum_Show;
			sendMessage(kAcquireOTReport);  
			OTnumberNow = para->OT_info.OTpageNum_Show;
		}

		if(para->parse.OvertimeDriveRecord.OTnumber != 0xFF)
		{
			if(uparrowLCDclear == 0)
			{
				LCD_Clear();
				uparrowLCDclear = 1;
			}
			/*超时驾驶记录*/
			displayChinese_16x16(0xB0,0x10,0x0,overTimeDriveRecord,1,2);
			displayChinese_16x16(0xB0,0x12,0x0,overTimeDriveRecord,5,8);
			sprintf(printString,"%03d",para->OT_info.OTpageNum_Show);
			ShowString(0xB0,0x16, 0x0,printString,12);
			memset(printString,0,200);
			memcpy(printString,para->parse.OvertimeDriveRecord.DriverLicenseNum,18);
			ShowString(0xB2,0x10, 0x0,printString,12);
			/*开始*/
			displayChinese_16x16(0xB4,0x10,0x0,OTBeginEndTime,4,5);
			sprintf(printString,"%02d.%02d.%02d %02d:%02d",para->parse.OvertimeDriveRecord.startTime.year,para->parse.OvertimeDriveRecord.startTime.month,para->parse.OvertimeDriveRecord.startTime.date,para->parse.OvertimeDriveRecord.startTime.h,para->parse.OvertimeDriveRecord.startTime.m);
			ShowString(0xB4,0x11, 0xc,printString,12);
			/*结束*/
			displayChinese_16x16(0xB6,0x10,0x0,OTBeginEndTime,8,9);
			sprintf(printString,"%02d.%02d.%02d %02d:%02d",para->parse.OvertimeDriveRecord.endTime.year,para->parse.OvertimeDriveRecord.endTime.month,para->parse.OvertimeDriveRecord.endTime.date,para->parse.OvertimeDriveRecord.endTime.h,para->parse.OvertimeDriveRecord.endTime.m);
			ShowString(0xB6,0x11, 0xc,printString,12);
		}
		
		else
		{
			if(noOTflag==0)
			{
				LCD_Clear();
				noOTflag=1;
			}
			/*无更多超时驾驶记录*/
			displayChinese_16x16(0xB3,0x11,0x0,overTimeDriveRecord,0,0);
			displayChinese_16x16(0xB3,0x11,0xc,more,0,1);
			displayChinese_16x16(0xB3,0x13,0x4,overTimeDriveRecord,1,2);
			displayChinese_16x16(0xB3,0x14,0xc,overTimeDriveRecord,5,8);
			uparrowLCDclear = 0;
		}
		
	}
	
	if(page1_row==0xB4)
	{
		/*满载*/
		displayChinese_16x16(0xB0,0x12,0x0,changeLoadingStatus,6,6);
		displayChinese_16x16(0xB0,0x12,0xc,changeLoadingStatus,2,2);
		
		/*空载*/
		displayChinese_16x16(0xB2,0x12,0x0,changeLoadingStatus,7,7);
		displayChinese_16x16(0xB2,0x12,0xc,changeLoadingStatus,2,2);
		
		sprintf(printString,"-->");
		ShowString(page2_row,0x10, 0x0,printString,12);
	}

	if(page_status!=page) LCD_Clear();
}

void page3_process(MCU_Parameters *para)
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
		switch(para->parse.rk_vehicle_info.car_plate_color)
		{
		case 0x1:
			displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,0,0);
			displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
			break;
		case 0x2:
			displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,1,1);
			displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
			break;
		case 0x3:
			displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,2,2);
			displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
			break;
		case 0x4:
			displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,3,3);
			displayChinese_16x16(0xB0,0x16,0xd,car_plate_color,4,4);
			break;
		default:
			displayChinese_16x16(0xB0,0x16,0x0,car_plate_color,5,7);
			break;
		}
			

		/*机动车号牌号码*/
		displayChinese_16x16(0xB2,0x10,0x0,Chinese_car_plate,0,4);
		displayChinese_16x16(0xB2,0x13,0xd,Chinese_car_plate,10,11);

		memset(carPlateString,0,20);
		plateHead = displayCarPlateHead(para->parse.rk_vehicle_info.car_plate_num);
		displayChinese_16x16(0xB4,0x10,0x0,car_plate_province,plateHead,plateHead);
		for(i=2;i<strlen(para->parse.rk_vehicle_info.car_plate_num);i++)
		{
			carPlateString[i-2] = para->parse.rk_vehicle_info.car_plate_num[i];
		}
		
		ShowString(0xB4,0x11, 0x0,carPlateString,12);
		displayChinese_16x16(0xB6,0x10,0x0,pulseRatio,0,3);
		sprintf(printString,"%d",para->parse.rk_vehicle_info.pulseRatio);
		ShowString(0xB6,0x14, 0x0,printString,12);


	}
	if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB2)
	{
		/*近光灯*/
		displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,7,8);
		displayChinese_16x16(0xB0,0x11,0x8,car_IO_status,13,13);
		if(para->mcu_car_info.low_beam == 1) displayIcon(0xB0,0x14, 0x0,nike);
		else displayIcon(0xB0,0x14, 0x0,icon_empty);
		
		/*右转向灯*/
		displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,9,9);
		displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,11,12);
		displayChinese_16x16(0xB2,0x12,0x4,car_IO_status,13,13);
		/*左转向灯*/
		displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,10,12);
		displayChinese_16x16(0xB4,0x12,0x5,car_IO_status,13,13);
		if(para->mcu_car_info.left == 1) displayIcon(0xB4,0x14, 0x0,nike);
		else displayIcon(0xB4,0x14, 0x0,icon_empty);
		
		
		/*制动*/
		displayChinese_16x16(0xB6,0x10,0x0,car_IO_status,14,15);
		if(para->mcu_car_info.brake == 1) displayIcon(0xB6,0x14, 0x0,nike);
		else displayIcon(0xB6,0x14, 0x0,icon_empty);
	}
	if(page1_row==0xB0 && page2_row==0xB0 && page3_row==0xB4)
	{
		/*远光灯*/
		displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,5,6);
		displayChinese_16x16(0xB0,0x11,0x8,car_IO_status,13,13);
		
		/*点火*/
		displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,16,17);
		if(para->mcu_car_info.fire == 1) displayIcon(0xB2,0x14, 0x0,nike);
		else displayIcon(0xB2,0x14, 0x0,icon_empty);

//		/*空挡*/
//		displayChinese_16x16(0xB2,0x10,0x0,car_IO_status,2,2);
//		displayChinese_16x16(0xB2,0x10,0xc,car_IO_status,4,4);
//		/*倒挡*/
//		displayChinese_16x16(0xB4,0x10,0x0,car_IO_status,3,4);
//		/*离合*/
//		displayChinese_16x16(0xB0,0x10,0x0,car_IO_status,0,1);

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
		memset(printString,0,200);
		memcpy(printString,para->ICcard_info.DriverLicenseNum,18);
		ShowString(0xB2,0x10, 0x0,printString,12);
	}

	if(page1_row==0xB2) page--;
	
	if(page1_row==0xB4) 
	{
		page--;
		if(page2_row==0xB0) para->mcu_car_info.LoadingStatus = 3;//满载
		if(page2_row==0xB2)	para->mcu_car_info.LoadingStatus = 0;//空载
	}

	if(page_status!=page)LCD_Clear();
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
            else if(key_text == KEY_up_down_PRES)
            {
                up_down_pressed++;
            }
			else;
        }
    }
}

