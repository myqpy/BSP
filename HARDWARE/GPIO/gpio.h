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
//#define 

//#define GPIO0_PRES 	1	//GPIO0����
//#define GPIO1_PRES	2	//GPIO1����
//#define GPIO2_PRES	3	//GPIO2����
//#define WKUP_PRES   4	//GPIO_UP����(��WK_UP/GPIO_UP)


void InPut_Init(void);//IO��ʼ��
u8 GPIO_Scan(void);  	//����ɨ�躯��					    
#endif
