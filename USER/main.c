#include "delay.h"
#include "displayLCD.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "menu.h"
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
#include "adc.h"

int main(void)
{

    u8 i=0;
    u8 time=0;
    u8 flag = 1;
    int drive_time=0;

    u8 canbuf[8];
    float volatageAD=0;
//	int h,m,s;

    ARM_selfCheck_info *rk_selfCheck_info = (ARM_selfCheck_info*)USART3_RX_BUF;
    ARM_time_info *time_info = (ARM_time_info*)USART3_RX_BUF;
    u8 mode = CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

    impulse_ratio = 570;
    car_info.header = 0xfe;

    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200

    printf("start up!!!!\r\n");
    usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART

    USART3_Init(115200);//3399ͨ�Ŵ���
    TIM3_ETR(impulse_ratio,0);//���岶���������ͳ�����
    TIM6_Int_Init(10000,7199);//�����������һ����
    Tim5_Int_Init(9, 7199);	//��ʱ��������һ����
    LcdInitial();//��ʾ��
    AT24CXX_Init();//IIC��ʼ������IC��
    UART4_init(115200);//��ӡ��
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13); //����ʾ������
	
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps
    InPut_Init();//�ⲿ������
    Adc_Init();
    RTC_Init(2023,4,23,15,54,50);	  			//RTC��ʼ��
//	RTC_Set(2023,4,23,16,39,10);
//    while(flag)
//    {
//        if(USART3_RX_STA&0X8000)    //���յ�����
//        {
//            USART3_RX_STA = USART3_RX_STA&0x7FFF;//��ȡ��ʵ���ַ�����
//            if(USART3_RX_BUF[0] == 0xFE)
//            {
//                printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",time_info->w_year, time_info->w_month, time_info->w_date,time_info->hour,time_info->min,time_info->sec);
//                RTC_Set(time_info->w_year, time_info->w_month, time_info->w_date, time_info->hour, time_info->min, time_info->sec);	//RTC��ʼ��
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
        MENU_processing(rk_selfCheck_info, drive_time,car_info.velocity);

        volatageAD = (float) (Get_Adc_Average(ADC_Channel_6,10) * 3.3 /4096) ;

        if(volatageAD <= 1.7)
        {
            GPIO_SetBits(GPIOF, GPIO_Pin_7);
        }
        if(volatageAD > 1.7)
        {
            GPIO_ResetBits(GPIOF, GPIO_Pin_7);
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

//          car_info.mileage = car_info.mileage + 1;
//          car_info.velocity = car_info.velocity + 1;
//			if(car_info.velocity>66)
//			{
//				car_info.velocity = 0;
//			}
//			car_info.status = GPIO_Scan();
//			car_info.brake;
            Usart_SendStr_length(USART3, (uint8_t*)&car_info, sizeof(ARM_vehicle_info));

        }



        if(USART3_RX_STA&0X8000)    //���յ�����
        {
            USART3_RX_STA = USART3_RX_STA&0x7FFF;//��ȡ��ʵ���ַ�����
            if(USART3_RX_BUF[0] == 0xEE)
            {
                memcpy(rk_selfCheck_info,USART3_RX_BUF, USART3_RX_STA);
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
			
			else if(USART3_RX_BUF[0] == 0xFE)
            {
                printf("%04d-%02d-%02d,%02d:%02d:%02d \r\n",time_info->w_year, time_info->w_month, time_info->w_date,time_info->hour,time_info->min,time_info->sec);
                RTC_Set(time_info->w_year, time_info->w_month, time_info->w_date, time_info->hour, time_info->min, time_info->sec);	//RTC��ʼ��
                for(i = 0; i<USART3_RX_STA; i++)
                {
                    printf("%02x ",USART3_RX_BUF[i]);
                }
                printf("\r\n");
            }


            USART3_RX_STA = 0;
        }



//        if(USART3_RX_STA&0X8000)    //���յ�����
//        {
//            USART3_RX_STA = USART3_RX_STA&0x7FFF;//��ȡ��ʵ���ַ�����
//            for(i = 0; i<USART3_RX_STA; i++)
//            {
//                printf("%c",USART3_RX_BUF[i]);
//            }
//            USART3_RX_STA = 0;
//        }

        if(Can_Receive_Msg(canbuf))//���յ�������
        {
            for(i = 0; i<sizeof(canbuf); i++)
            {
                printf("%c",canbuf[i]);
            }
        }
    }
}

