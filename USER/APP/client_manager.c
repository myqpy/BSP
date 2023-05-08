#include "terminal_parameter.h"
#include "client_manager.h"
#include <string.h>
#include "usart.h"
#include "usart3.h"
#include "24cxx.h" 
#include "bcd.h"
#include "util.h"
#include "packager.h"


u8 MsgIdCmd[5];
//u8 statusCmd[3];

MCU_Parameters para;

//unsigned int RealBufferSendSize = 0;

void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}




int packagingMessage(unsigned int msg_id)
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

int packagingSOSMessage(unsigned char statusbit, unsigned char value)
{

    para.packager.msg_id = 0x03; // 设置消息ID.
    if (McuSOSPackage(&para,statusbit,value) < 0)
    {
        printf("[jt808FramePackage]: FAILED !!!\r\n");
        return -1;
    }
	para.packager.msg_flow_num++; // 每正确生成一条命令, 消息流水号增加1.
    return 0;
}

int packagingOTMessage(unsigned char Page)
{

    para.packager.msg_id = 0x04; // 设置消息ID.
    if (McuOTPackage(&para,Page) < 0)
    {
        printf("[jt808FramePackage]: FAILED !!!\r\n");
        return -1;
    }
	para.packager.msg_flow_num++; // 每正确生成一条命令, 消息流水号增加1.
    return 0;
}





//void statusReport(unsigned char statusBit, unsigned char statusValue)
//{
//	statusCmd[0]= 0xDD;
//	statusCmd[1]= statusBit;
//	statusCmd[2]= statusValue;

//	Usart_SendStr_length(USART3, statusCmd, 3);
//}

//void GPS_FLASH_WRITE(void)
//{
//	FLASH_WriteByte(FLASH_ADDR, (uint8_t*)&parameter_.parse.terminal_parameters, sizeof(parameter_.parse.terminal_parameters));
//}


void ICcardRead(MCU_ICcard_info *driver_info)
{
	AT24CXX_Read(0,(u8*)&driver_info,sizeof(driver_info));
	
	printf("driver_info.driver_num: %s \r\n",(driver_info->DriverLicenseNum));
	printf("driver_info.year: 0x%02x \r\n",BcdToHex(driver_info->DriverLicenseValid_Year));
	printf("driver_info.month: 0x%02x \r\n",BcdToHex(driver_info->DriverLicenseValid_Month));
	printf("driver_info.day: 0x%02x \r\n",BcdToHex(driver_info->DriverLicenseValid_Date));
	printf("driver_info.XOR_word: 0x%02x \r\n",driver_info->BCCchecksum);
}



int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id)
{
    int result = 0;
    int i;
    for (i = 0; i < 4; ++i)
    {
        if (MsgIdCmd[i] == msg_id)
        {
            result = 1;
        }
    }
    return result;
}
