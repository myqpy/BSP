/*********SYSTEM headers***********/
#include "delay.h"
#include "sys.h"
#include <string.h>
#include <math.h>

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
#include "iwdg.h"

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

    uint8_t i=0, time=0, OTFlag = 0, receiveFlag = 0, ACC=0, powerOffFlag, carInfoFlag=0;
    extern u8 printer_cmd[200];
    u8 canbuf[8];
    float volatageAD=0;
	
    uint32_t powerOffTime=0;
	
	

//    ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;


    extern MCU_Parameters para;

    u8 mode = CAN_Mode_LoopBack;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式

//    para.parse.rk_vehicle_info.pulseRatio = 5700;
	para.mcu_car_info.isCharged = 1;
//	para.parse.parser.forbidTime = 1;
	
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200

    printf("start up!!!!\r\n");
    usmart_dev.init(SystemCoreClock/1000000);	//初始化USMART
	InPut_Init();//外部开关量

    USART3_Init(19200);//3399通信串口
//	while(1)
//	{
//		if(para.parse.rk_vehicle_info.pulseRatio!=0)
//		{
//			TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//脉冲捕获计数器，统计里程
//			break;
//		}
//    }
	
    TIM6_Int_Init(10000,7199);//脉冲计数器，一秒钟
    Tim5_Int_Init(9, 7199);	//定时计数器，一毫秒
    LcdInitial();//显示屏
	GPIO_SetBits(GPIOC,GPIO_Pin_13); //开显示屏背光
    AT24CXX_Init();//IIC初始化，读IC卡
    UART4_init(115200);//打印机
	ICcardWrite(0); //1写 0不写

    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);//CAN初始化环回模式,波特率500Kbps
   
    Adc_Init();
    RTC_Init(2000,1,1,0,0,0);	  			//RTC初始化
	
    while(1)
    {
        MENU_processing(&para);

        volatageAD = (float) (Get_Adc_Average(ADC_Channel_6,10) * 3.3 /4096) ;


        /*电瓶欠压*/
        if(volatageAD <= 1.7)
        {
            update_status(kLowVoltage, 1);
            sendMessage(kMCUAlarmReport);
            FLASH_WriteByte(FLASH_GPS_ADDR, (uint8_t*)&para.parse.Location_info, sizeof(para.parse.Location_info));
            system_reboot();
        }

        /*车辆熄火*/
        if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0))
        {
			powerOffFlag = 1;
            para.mcu_car_info.fire = 0;
			/*熄火达到1小时后进入低功耗模式*/
			if(powerOffTime>3600)
			{
				FLASH_WriteByte(FLASH_GPS_ADDR, (uint8_t*)&para.parse.Location_info, sizeof(para.parse.Location_info));
				system_reboot();
			}
        }
        else 
		{
			para.mcu_car_info.fire = 1;
			powerOffTime = 0;
			powerOffFlag = 0;
		}
		
		/*车辆制动*/
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)) para.mcu_car_info.brake = 0;
		else	para.mcu_car_info.brake = 1;
		
		/*车辆左转*/
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)) para.mcu_car_info.left = 0;
		else	para.mcu_car_info.left = 1;
		
		/*近光灯*/
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)) para.mcu_car_info.low_beam = 0;
		else	para.mcu_car_info.low_beam = 1;


        if(time!=calendar.sec)
        {
            time=calendar.sec;
			if(OTFlag == 1) para.mcu_car_info.drive_time++;
			if(powerOffFlag == 1) powerOffTime++;
			
//			ACC = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0);
//			printf("ACC:%02x\r\n",ACC);

//			printf("volatageAD:%f \r\n",volatageAD);
			printf("powerOffTime:%d \r\n",powerOffTime);

            if(para.mcu_car_info.drive_time >= 14400)
            {
                update_status(kOverTime, 1);
                sendMessage(kMCUAlarmReport);  
            }

            if(para.mcu_car_info.velocity>para.parse.rk_vehicle_info.speedLimit)
            {
                update_status(kOverSpeed, 1);
                sendMessage(kMCUAlarmReport);
            }
//			printf("time:%d\r\n",para.mcu_car_info.drive_time);
//			for(i = 0; i<12; i++)
//			{
//				printf("%02x ",para.parse.rk_vehicle_info.car_plate_num[i]);
//			}
//			printf("\r\n");
			printf("pulseRatio:%d\r\n",para.parse.rk_vehicle_info.pulseRatio);
//			printf("miles:%d\r\n",para.mcu_car_info.mileage);
			printf("speed:%d\r\n",para.mcu_car_info.velocity);
            sendMessage(kMCUStatusReport);
        }
		 

        if(USART3_RX_STA&0X8000)    //接收到数据
        {
            USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
			if((USART3_RX_BUF[0] == 0xEE) && (USART3_RX_BUF[USART3_RX_STA - 1] == 0xEE))
            {
//				for(i = 0; i<USART3_RX_STA; i++)
//				{
//					printf("%02x ",USART3_RX_BUF[i]);
//				}
//				printf("\r\n");
                parsingMessage(USART3_RX_BUF, USART3_RX_STA);
				
                switch(para.parse.parser.msg_id)
                {
				case kARMGeneralResponse:
				{
					printf("para.parse.parser.msg_id:0x%02x\r\n",para.parse.parser.msg_id);
					printf("para.parse.parser.msg_flow_num:%d\r\n",para.parse.parser.msg_flow_num);
					printf("para.parse.parser.msg_length:0x%02x\r\n",para.parse.parser.msg_length);	
				}
				case kArmOTrecord:
				{
//						for(i = 0; i<OvertimeDriveNum; i++)
//						{
//							pos = handle_overTimeRecord(USART3_RX_BUF, pos);
//		//					memset(printer_cmd ,0,sizeof(printer_cmd));
//		//					memcpy(printer_cmd,(u8 *)&rk_vehicle_info, sizeof(rk_vehicle_info));
//		//					Printer_printString(printer_cmd);
//							print_overTime_record_Body(OvertimeDriveRecord_info, i);
//							for(i = 0; i<sizeof(printer_cmd); i++)
//							{
//								printf("%02x ",printer_cmd[i]);
//							}
//							printf("\r\n");
//						}

				}
                case kTimeCorrect:
                {
					printf("TimeCorrect receive!!!\r\n");
					sendMessage(kMCUGeneralResponse);
					if(receiveFlag == 0)
					{
						RTC_Init(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTC初始化
						receiveFlag = 1;
					}
					if((para.parse.time_info.w_year!=calendar.w_year) || (para.parse.time_info.w_month!=calendar.w_month)||(para.parse.time_info.w_date!=calendar.w_date)||(para.parse.time_info.hour!=calendar.hour)||(para.parse.time_info.min!=calendar.min)) 
						RTC_Set(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTC校准
					if(abs(para.parse.time_info.sec - calendar.sec)>=3 && abs(para.parse.time_info.sec - calendar.sec)<=57)
						RTC_Set(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTC校准
                }
				break;
				case kSelfCheck:
				{
					printf("SelfCheck receive!!!\r\n");
//					printf("rk_info->SDStatus:%02x\r\n", para.parse.selfCheck_info.SDStatus);
//					printf("rk_info->EC20Status:%02x\r\n",para.parse.selfCheck_info.EC20Status);
//					printf("rk_info->EC20SignalStrength:%02x\r\n", para.parse.selfCheck_info.EC20SignalStrength);
//					printf("rk_info->cameraStatus:%02x\r\n", para.parse.selfCheck_info.cameraStatus);
//					printf("rk_info->velocityStatus:%02x\r\n", para.parse.selfCheck_info.velocityStatus);
//					printf("rk_info->BDStatus:%02x\r\n", para.parse.selfCheck_info.BDStatus);
					sendMessage(kMCUGeneralResponse);
				}
                break;
				
				case kCarInfo:
				{
					printf("CarInfo receive!!!\r\n");
					if(carInfoFlag==0)
					{
						TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//脉冲捕获计数器，统计里程
						carInfoFlag = 1;
					}
//					printf("para.parse.rk_vehicle_info.car_plate_num:%s\r\n", para.parse.rk_vehicle_info.car_plate_num);
//					printf("para.parse.rk_vehicle_info.car_plate_color:0x%02x\r\n", para.parse.rk_vehicle_info.car_plate_color);
//					printf("para.parse.rk_vehicle_info.speedLimit:0x%02x\r\n", para.parse.rk_vehicle_info.speedLimit);
//					printf("para.parse.rk_vehicle_info.pulseRatio:%d\r\n", para.parse.rk_vehicle_info.pulseRatio);

					sendMessage(kMCUGeneralResponse);
				}
				break;
				
				case kForbidTime:
				{
					printf("ForbidTime receive!!!\r\n");
					sendMessage(kMCUGeneralResponse);
				}
				break;
				
				case kLocation:
				{
//					for(i = 0; i<USART3_RX_STA; i++)
//					{
//						printf("%02x ",USART3_RX_BUF[i]);
//					}
//					printf("\r\n");
//					
//					printf("Location receive!!!\r\n");
//					// 报警标志 4B
//					printf("alarm %d\r\n", para.parse.Location_info.alarm);
//					// 状态位定义 4B
//					printf("status %d\r\n",para.parse.Location_info.status);
					// 纬度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//					printf("latitude %d\r\n",para.parse.Location_info.latitude);
//					// 经度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//					printf("longitude %d\r\n",para.parse.Location_info.longitude);
//					// 海拔高度, 单位为米(m) 2B
//					printf("altitude %d\r\n",para.parse.Location_info.altitude);
//					// 速度 1/10km/h 2B
//					printf("speed %d\r\n",para.parse.Location_info.speed);
//					// 方向 0-359,正北为0, 顺时针 2B
//					printf("bearing %d\r\n",para.parse.Location_info.bearing);
					
					sendMessage(kMCUGeneralResponse);
				}
				break;
				
				case kOTwarning:
				{
					printf("OTwarning received!!!\r\n");
					if(para.parse.parser.OTwarning == 1)
					{
						OTFlag = 1;
						para.mcu_car_info.drive_time = 12600;
					}
					else
					{
						OTFlag = 0;
						para.mcu_car_info.drive_time = 0;
					}
					sendMessage(kMCUGeneralResponse);
				}
				break;

                default:
                    break;
                }
            }
			USART3_RX_STA = 0;

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


