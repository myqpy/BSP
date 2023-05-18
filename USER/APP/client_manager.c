#include "terminal_parameter.h"
#include "client_manager.h"
#include <string.h>
#include "usart.h"
#include "usart3.h"
#include "24cxx.h" 
#include "bcd.h"
#include "util.h"
#include "packager.h"

MCU_Parameters para;


void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
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


int sendMessage(unsigned char msg_id)
{
	uint8_t i;
	packagingMessage(msg_id);
//	for(i=0;i<RealBufferSendSize;i++)
//	{
//		printf("%02x ",McuPackage[i]);
//    }
//	printf("\r\n");
	Usart_SendStr_length(USART3, (uint8_t*)&McuPackage, RealBufferSendSize);
	return 0;
}
