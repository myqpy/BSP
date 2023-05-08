/*********SYSTEM headers***********/
#include "delay.h"
#include "sys.h"
#include <string.h>

/*********BSP headers***********/
#include "rtc.h"
#include "ST7567a.h"
#include "usart.h"
#include "usmart.h"
#include "24cxx.h"
#include "adc.h"
#include "can.h"
#include "gpio.h"
#include "usart3.h"
#include "timer.h"
#include "bsp_internal_flash.h"   

/*********APP headers***********/
#include "displayLCD.h"
#include "menu.h"
#include "printercmd.h"
#include "printer.h"
#include "packager.h"
#include "parser.h"
#include "terminal_parameter.h"
#include "client_manager.h"
#include "OverTimeRecord.h"


int main(void)
{

    u8 i=0;
    u8 time=0;
//    u8 OvertimeDriveNum = 0;
    int drive_time=0;
    extern u8 printer_cmd[200];
    u8 canbuf[8];
    float volatageAD=0;
//	int h,m,s;
//	unsigned char *OvertimeDrive_ptr=NULL;
//    ARM_selfCheck_info *rk_selfCheck_info = (ARM_selfCheck_info*)USART3_RX_BUF;
//    ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;
//	MCU_Location_info *location_info = (MCU_Location_info*) USART3_RX_BUF;
//	ARM_vehicle_info rk_vehicle_info;
//	MCU_ICcard_info ICcard_info;
//	extern ARM_OvertimeDriveRecord_info OvertimeDriveRecord_info;
	extern MCU_Parameters para;
	
	
    u8 mode = CAN_Mode_LoopBack;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
//	u8 flag = 1;
//	uint8_t pos = 0;
    impulse_ratio = 570;
//    car_info.header = 0xfe;

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200

    printf("start up!!!!\r\n");
    usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART

    USART3_Init(115200);//3399通信串口
    TIM3_ETR(impulse_ratio,0);//脉冲捕获计数器，统计里程
    TIM6_Int_Init(10000,7199);//脉冲计数器，一秒钟
    Tim5_Int_Init(9, 7199);	//定时计数器，一毫秒
    LcdInitial();//显示屏
    AT24CXX_Init();//IIC初始化，读IC卡
    UART4_init(115200);//打印机

//	GPIO_ResetBits(GPIOC, GPIO_Pin_13); //关显示屏背光

    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);//CAN初始化环回模式,波特率500Kbps
    InPut_Init();//外部开关量
    Adc_Init();
    RTC_Init(2000,1,1,0,0,0);	  			//RTC初始化
	ICcardRead(&para.ICcard_info);
//	RTC_Set(2023,4,23,16,39,10);
//    while(flag)
//    {
//        if(USART3_RX_STA&0X8000)    //接收到数据
//        {
//            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
//            if(USART3_RX_BUF[0] == 0xFE)
//            {
//                printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",time_info->w_year, time_info->w_month, time_info->w_date,time_info->hour,time_info->min,time_info->sec);
//                RTC_Set(time_info->w_year, time_info->w_month, time_info->w_date, time_info->hour, time_info->min, time_info->sec);	//RTC初始化
//                flag = 0;
////				memset(USART3_TX_BUF,0,sizeof(USART3_TX_BUF));
////                USART3_TX_BUF[0] = 0xfe;
//                //USART3_TX_BUF[1] = 0xfe;
////                Usart_SendStr_length(USART3, USART3_TX_BUF, 1);
//            }
//            USART3_RX_STA = 0;
//        }
//    }

    while(1)
    {
        MENU_processing(&para.parse.selfCheck_info, drive_time,car_info.velocity);

        volatageAD = (float) (Get_Adc_Average(ADC_Channel_6,10) * 3.3 /4096) ;

        if(volatageAD <= 1.7)
        {
			McuSOSPackage(&para,kLowVoltage, 1);
			FLASH_WriteByte(FLASH_GPS_ADDR, (uint8_t*)&para.parse.Location_info, sizeof(para.parse.Location_info));
			system_reboot();
        }

        if(time!=calendar.sec)
        {
            time=calendar.sec;
            drive_time++;
//			printf("volatageAD:%f \r\n",volatageAD);
//			h=drive_time/3600;
//			m=drive_time%3600/60;
//			s=drive_time%60;
//			printf("%d \r\n",drive_time);
//			printf("%d %d %d \r\n \r\n",h,m,s);
//			printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);

			if(drive_time >= 14400) packagingSOSMessage(kOverTime, 1);
			if(car_info.velocity>para.parse.rk_vehicle_info.speedLimit) packagingSOSMessage(kOverSpeed, 1);
//			car_info.status = GPIO_Scan();
//			car_info.brake;
			
//            Usart_SendStr_length(USART3, (uint8_t*)&car_info, sizeof(ARM_vehicle_info));
			packagingMessage(kMCUStatusReport);

        }



        if(USART3_RX_STA&0X8000)    //接收到数据
        {
            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
            if((USART3_RX_BUF[0] == 0xEE) && (USART3_RX_BUF[USART3_RX_STA - 1] == 0xEE))
            {
				parsingMessage(USART3_RX_BUF, USART3_RX_STA);
				
				
//              memcpy(rk_selfCheck_info,USART3_RX_BUF, USART3_RX_STA);
//				printf("rk_info->op:%02x\r\n", rk_info->op);
//				printf("rk_info->SDStatus:%02x\r\n", rk_info->SDStatus);
//				printf("rk_info->EC20Status:%02x\r\n", rk_info->EC20Status);
//				printf("rk_info->EC20SignalStrength:%02x\r\n", rk_info->EC20SignalStrength);
//				printf("rk_info->cameraStatus:%02x\r\n", rk_info->cameraStatus);
//				printf("rk_info->velocityStatus:%02x\r\n", rk_info->velocityStatus);
//				printf("rk_info->BDStatus:%02x\r\n", rk_info->BDStatus);

                for(i = 0; i<USART3_RX_STA; i++)
                {
                    printf("%02x ",USART3_RX_BUF[i]);
                }
                printf("\r\n");

            }

//            else if(USART3_RX_BUF[0] == 0xFE)
//            {
//                printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",para.parse.time_info.w_year, time_info->w_month, time_info->w_date,time_info->hour,time_info->min,time_info->sec);
//                RTC_Set(time_info->w_year, time_info->w_month, time_info->w_date, time_info->hour, time_info->min, time_info->sec);	//RTC初始化
//                for(i = 0; i<USART3_RX_STA; i++)
//                {
//                    printf("%02x ",USART3_RX_BUF[i]);
//                }
//                printf("\r\n");
//            }
			
//			else if(USART3_RX_BUF[0] == 0xDD)
//            {
//				
//				// 报警标志 4B
//				printf("alarm %d",location_info->alarm);
//				// 状态位定义 4B
//				printf("status %d",location_info->status);
//				// 纬度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//				printf("latitude %d",location_info->latitude);
//				// 经度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//				printf("longitude %d",location_info->longitude);
//				// 海拔高度, 单位为米(m) 2B
//				printf("altitude %d",location_info->altitude);
//				// 速度 1/10km/h 2B
//				printf("speed %d",location_info->speed);
//				// 方向 0-359,正北为0, 顺时针 2B
//				printf("bearing %d",location_info->bearing);
//				// 时间, "YYMMDDhhmmss"(GMT+8时间, 本标准之后涉及的时间均采用此时区).12B
//				// std::string time;
//				printf("time %s",location_info->time);
//				
////                for(i = 0; i<USART3_RX_STA; i++)
////                {
////                    printf("%02x ",USART3_RX_BUF[i]);
////                }
//                printf("\r\n");
//            }


//            else if(USART3_RX_BUF[0] == 0xEF)
//            {	
//				OvertimeDriveNum = USART3_RX_BUF[1];
//				pos++;
//				for(i = 0; i<OvertimeDriveNum; i++)
//				{
//					pos = handle_overTimeRecord(USART3_RX_BUF, pos);
////					memset(printer_cmd ,0,sizeof(printer_cmd));
////					memcpy(printer_cmd,(u8 *)&rk_vehicle_info, sizeof(rk_vehicle_info));
////					Printer_printString(printer_cmd);
//					print_overTime_record_Body(OvertimeDriveRecord_info, i);
//					for(i = 0; i<sizeof(printer_cmd); i++)
//					{
//						printf("%02x ",printer_cmd[i]);
//					}
//					printf("\r\n");
//				}
				 
				
				
//                for(i = 0; i<USART3_RX_STA; i++)
//                {
//                    printf("%02x ",USART3_RX_BUF[i]);
//                }
//                printf("\r\n");
//            }


//            USART3_RX_STA = 0;
//        }



//        if(USART3_RX_STA&0X8000)    //接收到数据
//        {
//            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
//            for(i = 0; i<USART3_RX_STA; i++)
//            {
//                printf("%c",USART3_RX_BUF[i]);
//            }
//            USART3_RX_STA = 0;
//        }

        if(Can_Receive_Msg(canbuf))//接收到有数据
        {
            for(i = 0; i<sizeof(canbuf); i++)
            {
                printf("%c",canbuf[i]);
            }
        }
    }
}
}


