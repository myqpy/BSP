#include "terminal_parameter.h"
#include "client_manager.h"
#include <string.h>
#include "usart.h"
#include "usart3.h"
#include "24cxx.h" 
#include "bcd.h"


u8 statusCmd[3];

void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}


void statusReport(unsigned char statusBit, unsigned char statusValue)
{
	statusCmd[0]= 0xDD;
	statusCmd[1]= statusBit;
	statusCmd[2]= statusValue;

	Usart_SendStr_length(USART3, statusCmd, 3);
}

//void GPS_FLASH_WRITE(void)
//{
//	FLASH_WriteByte(FLASH_ADDR, (uint8_t*)&parameter_.parse.terminal_parameters, sizeof(parameter_.parse.terminal_parameters));
//}


void ICcardRead(MCU_ICcard_info driver_info)
{
	AT24CXX_Read(0,(u8*)&driver_info,sizeof(driver_info));
	
	printf("driver_info.driver_num: %s \r\n",(driver_info.DriverLicenseNum));
	printf("driver_info.year: 0x%02x \r\n",BcdToHex(driver_info.DriverLicenseValid_Year));
	printf("driver_info.month: 0x%02x \r\n",BcdToHex(driver_info.DriverLicenseValid_Month));
	printf("driver_info.day: 0x%02x \r\n",BcdToHex(driver_info.DriverLicenseValid_Date));
	printf("driver_info.XOR_word: 0x%02x \r\n",driver_info.BCCchecksum);
}

unsigned char BccCheckSum(const unsigned char *src, unsigned long len)
{
  unsigned char checksum = 0;
	unsigned long i;
  for (i = 0; i < len; ++i)
  {
    checksum = checksum ^ src[i];
  }
	#ifdef __JT808_DEBUG
		printf("[BccCheckSum] OK !\r\n");
	#endif
  return checksum;
}