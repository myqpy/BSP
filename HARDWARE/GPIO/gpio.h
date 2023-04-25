#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define GPIO0  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)
#define GPIO1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define GPIO2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define GPIO4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)


#define KEY0	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键0
#define KEY1	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define KEY2   	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键2 
#define KEY3   	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//读取按键3



#define KEY_confirmed_PRES	1		//KEY0  
#define KEY_DownArrow_PRES	2		//KEY1 
#define KEY_UpArrow_PRES	3		//KEY2  
#define KEY_menu_PRES	4			//KEY3  
#define KEY_up_down_PRES 5			//key ↑ + ↓

void InPut_Init(void);//IO初始化
u8 GPIO_Scan(void);  	//按键扫描函数	
u8 KEY_Scan(u8 mode);  	//按键扫描函数
#endif
