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
uint8_t powerOffFlag, OTFlag = 0, time=0,receiveFlag = 0, carInfoFlag=0, OTprintHeadFlag = 0;
uint32_t powerOffTime=0, timer4_1ms = 0;
float volatageAD=0;
uint8_t can_i=0;
u8 canRecvBuf[8];
u8 canSendBuf[8];


void bsp_init(void)
{
//	u8 mode = CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
	u8 mode = 0;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	InPut_Init();//�ⲿ������
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	USART3_Init(19200);//3399ͨ�Ŵ���
	TIM6_Int_Init(10000,7199);//�����������һ����
	Tim5_Int_Init(9, 7199);	//��ʱ��������һ����
	Timer4_Init(9,7199);//��ʱ��������һ����
	LcdInitial();//��ʾ��
	GPIO_SetBits(GPIOC,GPIO_Pin_13); //����ʾ������
	AT24CXX_Init();//IIC��ʼ������IC��
	UART4_init(115200);//��ӡ��
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,mode);//CAN��ʼ������ģʽ,������500Kbps
	Adc_Init();
	RTC_Init(2000,1,1,0,0,0);	  			//RTC��ʼ��
	para.OT_info.OTpageNum_Show = 1;
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
	/*��ƿǷѹ*/
	LowVoltage_process(volatageAD);

	/*����Ϩ��*/
	ACC_OFF_process();
	
	/*�����ƶ�*/
	Brake_process();
	
	/*������ת*/
	LeftSignal_process();
	
	/*�����*/
	LowBeam_process();
}

void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}

int packagingMessage(unsigned char msg_id)
{

    //���ҵ�ǰmsgID�Ƿ�����ڴ������ϢID������
    if (0 == findMsgIDFromTerminalPackagerCMD(msg_id))
    {
        printf("[findMsgIDFromTerminalPackagerCMD] no msg_id \r\n");
        return -1;
    }

    para.packager.msg_id = msg_id; // ������ϢID.
    if (McuMsgPackage(&para) < 0)
    {
        printf("[jt808FramePackage]: FAILED !!!\r\n");
        return -1;
    }
	
	para.packager.msg_flow_num++; // ÿ��ȷ����һ������, ��Ϣ��ˮ������1.
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
		/*Ϩ��ﵽ1Сʱ�����͹���ģʽ*/
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

		Can_Send_Msg(canSendBuf,8);
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
	
}

void checkCommand_E3_process(void)
{
	
}

void checkCommand_E4_process(void)
{
	
}


void Reception3399(void)
{
	if(USART3_RX_STA&0X8000)    //���յ�����
	{
		USART3_RX_STA = USART3_RX_STA&0x7FFF;//��ȡ��ʵ���ַ�����
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
				RTC_Init(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTC��ʼ��
				receiveFlag = 1;
			}
			if((para.parse.time_info.w_year!=calendar.w_year) || (para.parse.time_info.w_month!=calendar.w_month)||(para.parse.time_info.w_date!=calendar.w_date)||(para.parse.time_info.hour!=calendar.hour)||(para.parse.time_info.min!=calendar.min)) 
				RTC_Set(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTCУ׼
			if(abs(para.parse.time_info.sec - calendar.sec)>=3 && abs(para.parse.time_info.sec - calendar.sec)<=57)
				RTC_Set(para.parse.time_info.w_year, para.parse.time_info.w_month, para.parse.time_info.w_date, para.parse.time_info.hour, para.parse.time_info.min, para.parse.time_info.sec);	//RTCУ׼
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
				TIM3_ETR(para.parse.rk_vehicle_info.pulseRatio/10,0);//���岶���������ͳ�����
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
			
//			// ������־ 4B
//			printf("alarm %d\r\n", para.parse.Location_info.alarm);
//			// ״̬λ���� 4B
//			printf("status %d\r\n",para.parse.Location_info.status);
//			// γ��(�Զ�Ϊ��λ��γ��ֵ����10��6�η�, ��ȷ�������֮һ��) 4B
//			printf("latitude %d\r\n",para.parse.Location_info.latitude);
//			// ����(�Զ�Ϊ��λ��γ��ֵ����10��6�η�, ��ȷ�������֮һ��) 4B
//			printf("longitude %d\r\n",para.parse.Location_info.longitude);
//			// ���θ߶�, ��λΪ��(m) 2B
//			printf("altitude %d\r\n",para.parse.Location_info.altitude);
//			// �ٶ� 1/10km/h 2B
//			printf("speed %d\r\n",para.parse.Location_info.speed);
//			// ���� 0-359,����Ϊ0, ˳ʱ�� 2B
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
//	printf("%d\r\n",timer4_1ms);
	if(timer4_1ms >= 200)
	{
		sendMessage(kMCUStatusReport);
		timer4_1ms = 0;
	}
}

void Can_process()
{
	if(Can_Receive_Msg(canRecvBuf))//���յ�������
	{
		for(can_i = 0; can_i<sizeof(canRecvBuf); can_i++)
		{
			printf("%02x ",canRecvBuf[can_i]);
		}
		printf("\r\n");
	}
}

void TIM4_IRQHandler(void)
{ 		    	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
        timer4_1ms ++;
    }  
}
