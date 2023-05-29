#include "sys.h"
#include "usart.h"
#include "menu.h"
#include "client_manager.h"

uint8_t test;

int main(void)
{
	extern MCU_Parameters para;
//  ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;
//	para.parse.rk_vehicle_info.pulseRatio = 5700;
//	para.parse.parser.forbidTime = 1;
//  usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART
//	TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//���岶���������ͳ�����
	bsp_init();
	ICcardWrite(0); //1д 0��д
	printf("start up!!!!\r\n");
	
//	GPIO_SetBits(GPIOA,GPIO_Pin_15);
//	test = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
//	printf("PA15: %02x",test);
	
	while(1)
	{
		/*�˵�����*/
		/*��������Ļ��ʾ*/
		/*������ӡ����*/
		MENU_processing(&para);

		/*����������*/
		Input_process();

		/*һ��һ֡�������в���*/
		actionEverySecond();

		/*����3399*/
		Reception3399();

		/*����can��������*/
		Can_process();
		
		/*0.2��һ�η��ͳ���״̬��Ϣ*/
		CarStatusReport();
	}
}


