#include "stm32f10x.h"
#include "gpio.h"
#include "sys.h" 
#include "delay.h"
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
								    
//������ʼ������

void InPut_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD ,ENABLE);//ʹ��PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//GPIO0-GPIO2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4,5
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTDʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;//GPIO0-GPIO4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	//PF-7 init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
	//PF-8 init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 			 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	

}

//GPIO�������������ĸ�GPIO�����״ֵ̬
u8 GPIO_Scan()
{	 
	u8 status = GPIO0<<3 | GPIO1 <<2 | GPIO2<<1 | GPIO4;
	return status;
}

u8 KEY_Scan(u8 mode)
{
    static u8 key_up=0;//�������ɿ���־
    if(mode) key_up=1;  //֧������
	
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
    {
        delay_ms(100);//ȥ����
        key_up=0;
		
        if(KEY0==0)return KEY_confirmed_PRES;
		
		else if ((KEY1==0)&&(KEY2==0)) return KEY_up_down_PRES;
			
		else if(KEY2==0)return KEY_UpArrow_PRES;
		
        else if(KEY1==0)return KEY_DownArrow_PRES;
        
		else if(KEY3==0)return KEY_menu_PRES;
		
		
    } 
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1) key_up=1;
//	else if(KEY0==0&&KEY1==0&&KEY2==0&&KEY3==0) key_up=1;
	
    return 0;// �ް�������
}
