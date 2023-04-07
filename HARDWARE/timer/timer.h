#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "terminal_parameter.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
extern	uint32_t distance;
extern	uint16_t pluse_counter_2;
extern	uint16_t pluse_counter_1;
extern	uint32_t velocity;
extern 	uint32_t impulse_ratio;
extern	car_info_t car_info;
void TIM3_ETR(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);
#endif
