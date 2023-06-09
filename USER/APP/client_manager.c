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
#include "iwdg.h"
#include "bsp_internal_flash.h"   

/*********APP headers***********/
#include "displayLCD.h"
#include "menu.h"
#include "bcd.h"
#include "util.h"
#include "printercmd.h"
#include "printer.h"
#include "packager.h"
#include "parser.h"
#include "terminal_parameter.h"
#include "client_manager.h"
#include "OverTimeRecord.h"

MCU_Parameters para;
uint8_t powerOffFlag, OTFlag = 0, time=0,receiveFlag = 0, carInfoFlag=0, OTprintHeadFlag = 0, WakeupSendFlag = 0;
uint32_t powerOffTime=0, timer4_1ms = 0;
float volatageAD=0;
uint8_t can_i=0;
u8 canRecvBuf[8];
u8 canSendBuf[8];
extern CanRxMsg RxMessage;

void bsp_init(void)
{
//	u8 mode = CAN_Mode_LoopBack;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
	u8 mode = 0;
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	myGPIO_Init();//GPIO配置
	uart_init(115200);	 	//串口初始化为115200
	USART3_Init(19200);//3399通信串口
	TIM6_Int_Init(10000,7199);//脉冲计数器，一秒钟
	Tim5_Int_Init(9, 7199);	//定时计数器，一毫秒
	Timer4_Init(9,7199);//定时计数器，一毫秒
	LcdInitial();//显示屏
	GPIO_SetBits(GPIOC,GPIO_Pin_13); //开显示屏背光
	AT24CXX_Init();//IIC初始化，读IC卡
	UART4_init(115200);//打印机
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);//CAN初始化环回模式,波特率500Kbps
	Adc_Init();
	RTC_Init(2000,1,1,0,0,0);	  			//RTC初始化
	para.OT_info.OTpageNum_Show = 1;
	
	para.parse.parser.checkCommand = 0xE3;
	para.mcu_car_info.isCharged = 1;
	
	canSendBuf[0] = 0x0;
	canSendBuf[1] = 0x1;
	canSendBuf[2] = 0x2;
	canSendBuf[3] = 0x3;
	canSendBuf[4] = 0x4;
	canSendBuf[5] = 0x5;
	canSendBuf[6] = 0x6;
	canSendBuf[7] = 0x7;
}

void Input_process(void)
{
	volatageAD = (float) (Get_Adc_Average(ADC_Channel_6,10) * 3.3 /4096) ;
	/*电瓶欠压*/
	LowVoltage_process(volatageAD);

	/*车辆熄火*/
	ACC_OFF_process();
	
	/*车辆制动*/
	Brake_process();
	
	/*车辆左转*/
	LeftSignal_process();
	
	/*近光灯*/
	LowBeam_process();
}

void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}

int sendMessage(unsigned char msg_id)
{
	
	packagingMessage(msg_id);

	#ifdef __STM32_DEBUG	
	uint8_t packcount;
	for(packcount = 0; packcount<RealBufferSendSize; packcount++)
	{
		printf("%02x ",McuPackage[packcount]);
	}
	printf("\r\n");
	#endif
	
	Usart_SendStr_length(USART3, (uint8_t*)&McuPackage, RealBufferSendSize);
	return 0;
}

int packagingMessage(unsigned char msg_id)
{

    //查找当前msgID是否存在于待打包消息ID数组中
    if (0 == findMsgIDFromTerminalPackagerCMD(msg_id))
    {
        printf("[findMsgIDFromTerminalPackagerCMD] no msg_id \r\n");
        return -1;
    }

    para.packager.msg_id = msg_id; // 设置消息ID.
    if (McuMsgPackage(&para) < 0)
    {
        printf("[jt808FramePackage]: FAILED !!!\r\n");
        return -1;
    }
	
	para.packager.msg_flow_num++; // 每正确生成一条命令, 消息流水号增加1.
    return 0;
}




void ICcardRead()
{
	AT24CXX_Read(0,(u8*)&para.ICcard_info,sizeof(para.ICcard_info));
	
	printf("driver_info.driver_num: %s \r\n",(para.ICcard_info.DriverLicenseNum));
	printf("driver_info.year: 0x%02x \r\n",BcdToHex(para.ICcard_info.DriverLicenseValid_Year));
	printf("driver_info.month: 0x%02x \r\n",BcdToHex(para.ICcard_info.DriverLicenseValid_Month));
	printf("driver_info.day: 0x%02x \r\n",BcdToHex(para.ICcard_info.DriverLicenseValid_Date));
	printf("driver_info.XOR_word: 0x%02x \r\n",para.ICcard_info.BCCchecksum);
}

void ICcardWrite(uint8_t writeOrNah)
{
	if (writeOrNah ==1)
	{
		printf("ICcardWrite!!!! \r\n");
		memset((u8*)&para.ICcard_info,0,sizeof(para.ICcard_info));
		memcpy(para.ICcard_info.DriverLicenseNum,"410105199607150035",sizeof("410105199607150035"));
		para.ICcard_info.DriverLicenseValid_Year = HexToBcd(0x33);
		para.ICcard_info.DriverLicenseValid_Month = HexToBcd(0x12);
		para.ICcard_info.DriverLicenseValid_Date = HexToBcd(0x12);
		para.ICcard_info.BCCchecksum = BccCheckSum((u8*)&para.ICcard_info, sizeof(para.ICcard_info));
		AT24CXX_Write(0,(u8*)&para.ICcard_info,sizeof(para.ICcard_info));
	}
}

void update_status(unsigned char statusbit, unsigned char value)
{
	para.packager.statusBit = statusbit;
	para.packager.statusValue = value;
}




void LowVoltage_process(float voltage)
{
	if(voltage <= 1.7)
	{
		update_status(kLowVoltage, 1);
		sendMessage(kMCUAlarmReport);
		FLASH_WriteByte(FLASH_GPS_ADDR, (uint8_t*)&para.parse.Location_info, sizeof(para.parse.Location_info));
		system_reboot();
	}
}

uint8_t ACC_OFF_process(void)
{
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
	return powerOffFlag;
}

void actionEverySecond(void)
{
	if(time!=calendar.sec)
	{
		time=calendar.sec;
		if(OTFlag == 1) para.mcu_car_info.drive_time++;
		if(powerOffFlag == 1) powerOffTime++;
		
//		ACC = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0);
//		printf("ACC:%02x\r\n",ACC);

//		printf("volatageAD:%f \r\n",volatageAD);
//		printf("time:%d\r\n",para.mcu_car_info.drive_time);
//		for(i = 0; i<12; i++)
//		{
//			printf("%02x ",para.parse.rk_vehicle_info.car_plate_num[i]);
//		}
//		printf("\r\n");
//		printf("pulseRatio:%d\r\n",para.parse.rk_vehicle_info.pulseRatio);
//		printf("miles:%d\r\n",para.mcu_car_info.mileage);
//		printf("speed:%d\r\n",para.mcu_car_info.velocity);
//		printf("powerOffTime:%d \r\n",powerOffTime);

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

		if(WakeupSendFlag == 1)
		{
			sendMessage(kMCUWakeup);
		}
//		Can_Send_Msg(canSendBuf,8);
		
		/******检定命令字*******/
		checkCommand_process();
	}
}

void setThreeAndHalfHourFlag(void)
{
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
}

void Brake_process(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)) para.mcu_car_info.brake = 0;
	else	para.mcu_car_info.brake = 1;
}
void LeftSignal_process(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)) para.mcu_car_info.left = 0;
	else	para.mcu_car_info.left = 1;
}
void LowBeam_process(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)) para.mcu_car_info.low_beam = 0;
	else	para.mcu_car_info.low_beam = 1;
}

void checkCommand_process(void)
{
	switch(para.parse.parser.checkCommand)
	{
	case(0xE2):
		checkCommand_E2_process();
		break;
	case(0xE3):
		checkCommand_E3_process();
		break;
	case(0xE4):
		checkCommand_E4_process();
		break;
	default:
		break;
	}
}

void checkCommand_E2_process(void)
{
	return;
}

void checkCommand_E3_process(void)
{
	switch(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))
	{
		case 0:
			GPIO_SetBits(GPIOA,GPIO_Pin_15);
			break;
		case 1:
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			break;
	}
	return;
}

void checkCommand_E4_process(void)
{
	return;
}


void Reception3399(void)
{
	if(USART3_RX_STA&0X8000)    //接收到数据
	{
		USART3_RX_STA = USART3_RX_STA&0x7FFF;//获取到实际字符数量
		Parse3399(USART3_RX_BUF, USART3_RX_STA);
		memset(USART3_RX_BUF,0,USART3_RX_STA);
		USART3_RX_STA = 0;
	}
}

void Parse3399(u8* receiveBuf,u16 length)
{
	if((receiveBuf[0] == 0xEE) && (receiveBuf[length-1] == 0xEE))
	{
		
		#ifdef __STM32_DEBUG
		uint8_t rk_i;		
		for(rk_i = 0; rk_i<length; rk_i++)
		{
			printf("%02x ",receiveBuf[rk_i]);
		}
		printf("\r\n");
		#endif
		
		
		
		
		parsingMessage(receiveBuf, length);
		
		switch(para.parse.parser.msg_id)
		{
		case kARMGeneralResponse:
		{
			#ifdef __STM32_DEBUG
			printf("kARMGeneralResponse receive!!!\r\n");
			printf("para.parse.parser.response_id:0x%02x\r\n",para.parse.parser.response_id);
			printf("para.parse.parser.response_flow_num:%d\r\n",para.parse.parser.response_flow_num);
			printf("para.parse.parser.response_result:0x%02x\r\n",para.parse.parser.response_result);
			#endif
			
			if(para.parse.parser.response_id == kMCUWakeup)
			{
				WakeupSendFlag = 0;
			}
		}
		break;
		case kArmOTrecord:
		{
			sendMessage(kMCUGeneralResponse);
			printf("kArmOTrecord receive!!!\r\n");
			OT_print_process();
		}
		break;
		case kTimeCorrect:
		{
			sendMessage(kMCUGeneralResponse);
			#ifdef __STM32_DEBUG
			printf("TimeCorrect receive!!!\r\n");	
			#endif			
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
			sendMessage(kMCUGeneralResponse);
			#ifdef __STM32_DEBUG
			printf("SelfCheck receive!!!\r\n");
//			printf("rk_info->SDStatus:%02x\r\n", para.parse.selfCheck_info.SDStatus);
//			printf("rk_info->EC20Status:%02x\r\n",para.parse.selfCheck_info.EC20Status);
//			printf("rk_info->EC20SignalStrength:%02x\r\n", para.parse.selfCheck_info.EC20SignalStrength);
//			printf("rk_info->cameraStatus:%02x\r\n", para.parse.selfCheck_info.cameraStatus);
//			printf("rk_info->velocityStatus:%02x\r\n", para.parse.selfCheck_info.velocityStatus);
//			printf("rk_info->BDStatus:%02x\r\n", para.parse.selfCheck_info.BDStatus);
			#endif
		}
		break;
		
		case kCarInfo:
		{
			sendMessage(kMCUGeneralResponse);
			printf("CarInfo receive!!!\r\n");
			if(carInfoFlag==0)
			{
				TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//脉冲捕获计数器，统计里程
				carInfoFlag = 1;
			}
			#ifdef __STM32_DEBUG
//			printf("para.parse.rk_vehicle_info.car_plate_num:%s\r\n", para.parse.rk_vehicle_info.car_plate_num);
//			printf("para.parse.rk_vehicle_info.car_plate_color:0x%02x\r\n", para.parse.rk_vehicle_info.car_plate_color);
//			printf("para.parse.rk_vehicle_info.speedLimit:0x%02x\r\n", para.parse.rk_vehicle_info.speedLimit);
//			printf("para.parse.rk_vehicle_info.pulseRatio:%d\r\n", para.parse.rk_vehicle_info.pulseRatio);
			#endif
		}
		break;
		
		case kForbidTime:
		{
			sendMessage(kMCUGeneralResponse);
			printf("ForbidTime receive!!!\r\n");		
		}
		break;
		
		case kLocation:
		{		
			sendMessage(kMCUGeneralResponse);	
#ifdef __STM32_DEBUG			
			printf("Location receive!!!\r\n");
			
//			// 报警标志 4B
//			printf("alarm %d\r\n", para.parse.Location_info.alarm);
//			// 状态位定义 4B
//			printf("status %d\r\n",para.parse.Location_info.status);
//			// 纬度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//			printf("latitude %d\r\n",para.parse.Location_info.latitude);
//			// 经度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
//			printf("longitude %d\r\n",para.parse.Location_info.longitude);
//			// 海拔高度, 单位为米(m) 2B
//			printf("altitude %d\r\n",para.parse.Location_info.altitude);
//			// 速度 1/10km/h 2B
//			printf("speed %d\r\n",para.parse.Location_info.speed);
//			// 方向 0-359,正北为0, 顺时针 2B
//			printf("bearing %d\r\n",para.parse.Location_info.bearing);	
#endif
		}
		break;
		
		case kOTwarning:
		{
			sendMessage(kMCUGeneralResponse);
			printf("OTwarning received!!!\r\n");
			setThreeAndHalfHourFlag();
		}
		break;
		
		case kZeroMileage:
		{
			#ifdef __STM32_DEBUG	
			printf("ZeroMileage received!!!\r\n");
			#endif
			sendMessage(kMCUGeneralResponse);
			if(para.parse.parser.zeroMileage == 1) para.mcu_car_info.mileage = 0;
		}
		break;

		case kcheckCommand:
		{
			#ifdef __STM32_DEBUG	
			printf("checkCommand received!!!\r\n");
			#endif
			sendMessage(kMCUGeneralResponse);
			
		}
		break;
		
		case kAwakeOver:
		{
			#ifdef __STM32_DEBUG	
			printf("AwakeOver received!!!\r\n");
			#endif
			sendMessage(kMCUGeneralResponse);
			para.parse.WakeUp.WakeUpMode_MCU.value = 0;
			FLASH_WriteByte(FLASH_WakeUp_ADDR, (uint8_t*)&para.parse.WakeUp, sizeof(para.parse.WakeUp));
			system_reboot();
		}
		break;
		
		
		default:
			break;
		}
	}	
}

void OT_print_process(void)
{
	if(para.OT_info.print_flag == 1)
	{
		printf("para->parse.OvertimeDriveRecord.OTnumber:0x%02x\r\n",para.parse.OvertimeDriveRecord.OTnumber);
		if(para.parse.OvertimeDriveRecord.OTnumber != 0xFF)
		{
			if(OTprintHeadFlag == 0)
			{
				print_overTime_record_Header(&para);
				OTprintHeadFlag = 1;
				
			}
			print_overTime_record_Body(&para, para.packager.OTpageNum);
			para.OT_info.OTpageNum_print++;
			para.packager.OTpageNum = para.OT_info.OTpageNum_print;
			
			printf("para->parse.OvertimeDriveRecord.DriverLicenseNum:%s\r\n",para.parse.OvertimeDriveRecord.DriverLicenseNum);
			printf("%02d-%02d-%02d,%02d:%02d:%02d\r\n",para.parse.OvertimeDriveRecord.startTime.year, para.parse.OvertimeDriveRecord.startTime.month, para.parse.OvertimeDriveRecord.startTime.date, para.parse.OvertimeDriveRecord.startTime.h,para.parse.OvertimeDriveRecord.startTime.m,para.parse.OvertimeDriveRecord.startTime.s);
			printf("%02d-%02d-%02d,%02d:%02d:%02d\r\n",para.parse.OvertimeDriveRecord.endTime.year, para.parse.OvertimeDriveRecord.endTime.month, para.parse.OvertimeDriveRecord.endTime.date, para.parse.OvertimeDriveRecord.endTime.h,para.parse.OvertimeDriveRecord.endTime.m,para.parse.OvertimeDriveRecord.endTime.s);
			sendMessage(kAcquireOTReport);
		}
		else 
		{
			para.OT_info.print_flag = 0;
			OTprintHeadFlag = 0;
			print_overTime_Autograph();
		}
	}
}

void CarStatusReport()
{
	if(timer4_1ms >= 200)
	{
		sendMessage(kMCUStatusReport);
		timer4_1ms = 0;
	}
}

void Can_process()
{
	if(Can_Receive_Msg(canRecvBuf))//接收到有数据
	{
		for(can_i = 0; can_i<sizeof(canRecvBuf); can_i++)
		{
			printf("%02x ",canRecvBuf[can_i]);
		}
		printf("\r\n");
	
		para.CAN_info.CAN_id.value = 0;
		if(RxMessage.IDE == CAN_Id_Standard) 
		{
			para.CAN_info.CAN_id.bit.CAN_IDE = 0;					//标准帧
			para.CAN_info.CAN_id.bit.CAN_Channel = 0; 				//通道1
			para.CAN_info.CAN_id.bit.CAN_dataCollect=0;				//原始数据
			para.CAN_info.CAN_id.bit.CAN_stdID = RxMessage.StdId;	//id赋值
		}

		if(RxMessage.IDE == CAN_Id_Extended) 
		{
			para.CAN_info.CAN_id.bit.CAN_IDE = 1;					//扩展帧
			para.CAN_info.CAN_id.bit.CAN_Channel = 0; 				//通道1
			para.CAN_info.CAN_id.bit.CAN_dataCollect=0;				//原始数据
			para.CAN_info.CAN_id.bit.CAN_stdID = RxMessage.StdId;	//id赋值
			para.CAN_info.CAN_id.bit.CAN_extID = RxMessage.ExtId;	//id赋值（扩展）
		}

		memcpy(para.CAN_info.CAN_data,RxMessage.Data,8);

		para.packager.CANnum = 1;
		sendMessage(kMCUCANReport);
	}
	
}

void WakeUpRead()
{
	Internal_ReadFlash(FLASH_WakeUp_ADDR, (uint8_t*)&para.parse.WakeUp, sizeof(para.parse.WakeUp));
	if(para.parse.WakeUp.WakeUpMode_MCU.value!=0)
	{
		WakeupSendFlag = 1;
	}
}

void awakeOver_process()
{
	if(para.parse.parser.awakeOver == 1)
	{
		para.parse.WakeUp.WakeUpMode_MCU.value = 0;
		system_reboot();
	}
}


void TIM4_IRQHandler(void)
{ 		    	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
        timer4_1ms ++;
    }  
}
