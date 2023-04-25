#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define GPIO0  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)
#define GPIO1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define GPIO2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
#define GPIO4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)


#define KEY0	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����0
#define KEY1	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����1
#define KEY2   	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����2 
#define KEY3   	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//��ȡ����3



#define KEY_confirmed_PRES	1		//KEY0  
#define KEY_DownArrow_PRES	2		//KEY1 
#define KEY_UpArrow_PRES	3		//KEY2  
#define KEY_menu_PRES	4			//KEY3  
#define KEY_up_down_PRES 5			//key �� + ��

void InPut_Init(void);//IO��ʼ��
u8 GPIO_Scan(void);  	//����ɨ�躯��	
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��
#endif
