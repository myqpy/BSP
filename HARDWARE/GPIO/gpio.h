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
//#define 

//#define GPIO0_PRES 	1	//GPIO0按下
//#define GPIO1_PRES	2	//GPIO1按下
//#define GPIO2_PRES	3	//GPIO2按下
//#define WKUP_PRES   4	//GPIO_UP按下(即WK_UP/GPIO_UP)


void InPut_Init(void);//IO初始化
u8 GPIO_Scan(void);  	//按键扫描函数					    
#endif
