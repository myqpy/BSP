/*********SYSTEM headers***********/
#include "delay.h"
#include "sys.h"
#include <string.h>
#include <math.h>

/*********BSP headers***********/
//#include "rtc.h"
//#include "ST7567a.h"
#include "usart.h"
//#include "usmart.h"
//#include "24cxx.h"
#include "adc.h"
#include "can.h"
//#include "gpio.h"
//#include "usart3.h"
//#include "timer.h"
//#include "iwdg.h"

/*********APP headers***********/
//#include "displayLCD.h"
#include "menu.h"
//#include "printercmd.h"
//#include "printer.h"
//#include "packager.h"
//#include "parser.h"
//#include "terminal_parameter.h"
#include "client_manager.h"
//#include "OverTimeRecord.h"


int main(void)
{
//    uint8_t i=0;
//	extern u8 printer_cmd[200];
//    u8 canbuf[8];
    
    extern MCU_Parameters para;
	para.packager.OTpageNum = 1;
	para.mcu_car_info.isCharged = 1;
//    ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;
//	para.parse.rk_vehicle_info.pulseRatio = 5700;
//	para.parse.parser.forbidTime = 1;
//    usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART
//	TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//脉冲捕获计数器，统计里程
	bsp_init();
	ICcardWrite(0); //1写 0不写
	printf("start up!!!!\r\n");
   
	
    while(1)
    {
		/*菜单处理*/
        MENU_processing(&para);

		/*开关量处理*/
		Input_process();

		/*一秒一帧做的所有操作*/
		actionEverySecond();

		/*解析3399*/
		Reception3399();

//		if(Can_Receive_Msg(canbuf))//接收到有数据
//		{
//			for(i = 0; i<sizeof(canbuf); i++)
//			{
//				printf("%c",canbuf[i]);
//			}
//		}
	
    }
}


