#include "sys.h"
#include "usart.h"
#include "menu.h"
#include "client_manager.h"
//#include <string.h>
//#include "bsp_internal_flash.h"   


//uint8_t test;

int main(void)
{
	extern MCU_Parameters para;
//  ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;
//	para.parse.rk_vehicle_info.pulseRatio = 5700;
//	para.parse.parser.forbidTime = 1;
//  usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART
//	TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//脉冲捕获计数器，统计里程
	bsp_init();
	ICcardWrite(0); //1写 0不写
	printf("start up!!!!\r\n");
	WakeUpRead();	//是否为意外唤醒检查
//	GPIO_SetBits(GPIOA,GPIO_Pin_15);
//	test = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
//	printf("PA15: %02x",test);
//	memset((uint8_t*)&para.parse.WakeUp,0,sizeof(para.parse.WakeUp));
//	FLASH_WriteByte(FLASH_WakeUp_ADDR, (uint8_t*)&para.parse.WakeUp, sizeof(para.parse.WakeUp));
	
	while(1)
	{
		/*菜单处理*/
		/*包含主屏幕显示*/
		/*包含打印功能*/
		MENU_processing(&para);

		/*开关量处理*/
		Input_process();

		/*一秒一帧做的所有操作*/
		actionEverySecond();

		/*解析3399*/
		Reception3399();

		/*解析can总线数据*/
		Can_process();
		
		/*0.2秒一次发送车辆状态信息*/
		CarStatusReport();
		
		/*处理结束唤醒*/
		awakeOver_process();
	}
}


