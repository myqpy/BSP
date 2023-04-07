
#include "delay.h"
#include "displayLCD.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"
#include "ST7567a.h"
#include "timer.h"
#include "24cxx.h"
#include "printer.h"
#include "terminal_parameter.h"
#include "usart3.h"
#include "can.h"
#include "gpio.h"
#include <string.h>

int main(void)
{
	
    u8 i=0;
    u8 time=0;
    
    u8 printer_cmd[200];
    u8 flag = 1;
    u8 canbuf[8];
    time_t *time_info = (time_t*)USART3_RX_BUF;
    u8 mode = CAN_Mode_LoopBack;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式

    impulse_ratio = 570;
    car_info.op = 0xfe;

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200

    printf("start up!!!!\r\n");
    usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART

    USART3_Init(115200);//3399通信串口
    TIM3_ETR(impulse_ratio,0);//脉冲捕获计数器，统计里程
    TIM6_Int_Init(10000,7199);//脉冲计数器，一秒钟
    LcdInitial();//显示屏
    AT24CXX_Init();//IIC初始化，读IC卡
    printer_init(115200);//打印机
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps
    InPut_Init();//外部开关量
    while(AT24CXX_Check())
    {
        printf("card don't insert\r\n");
        i++;
        if(i > 5)
            break;
        delay_ms(200);
    }
	RTC_Init(2023,4,7,15,13,55);	  			//RTC初始化
//    while(flag)
//    {
//        if(USART3_RX_STA&0X8000)    //接收到数据
//        {
//            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
//            if(USART3_RX_BUF[0] == 0xFE)
//            {
//                printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",time_info->w_year, time_info->w_month, time_info->w_date,time_info->hour,time_info->min,time_info->sec);
//                RTC_Init(time_info->w_year, time_info->w_month, time_info->w_date, time_info->hour, time_info->min, time_info->sec);	//RTC初始化
//                flag = 0;
////				memset(USART3_TX_BUF,0,sizeof(USART3_TX_BUF));
//                USART3_TX_BUF[0] = 0xfe;
//                //USART3_TX_BUF[1] = 0xfe;
//                Usart_SendStr_length(USART3, USART3_TX_BUF, 1);
//            }
//            USART3_RX_STA = 0;
//        }
//    }
    while(1)
    {
        if(time!=calendar.sec)
        {
            time=calendar.sec;
			showMainMenu();

            car_info.mileage = car_info.mileage + 1;
            car_info.velocity = car_info.velocity + 1;
//			car_info.status = GPIO_Scan();
//			car_info.brake;
            Usart_SendStr_length(USART3, (uint8_t*)&car_info, sizeof(car_info_t));

        }
        if(USART3_RX_STA&0X8000)    //接收到数据
        {
            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
            for(i = 0; i<USART3_RX_STA; i++)
            {
                printf("%c",USART3_RX_BUF[i]);
            }
            USART3_RX_STA = 0;
        }
        if(Can_Receive_Msg(canbuf))//接收到有数据
        {
            for(i = 0; i<sizeof(canbuf); i++)
            {
                printf("%c",canbuf[i]);
            }
        }
    }
}

