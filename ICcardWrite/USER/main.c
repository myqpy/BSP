#include "sys.h"
#include "24cxx.h" 
#include "usart.h"	  
#include "string.h"
#include "delay.h"
#include "bcd.h"

unsigned char BccCheckSum(const unsigned char *src, unsigned long len);

int main(void)
{	
	driver_info_t driver_info;
	delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
//	AT24CXX_Init();
	
//	memset((u8*)&driver_info,0,sizeof(driver_info));
//	memcpy(driver_info.driver_num,"410105199607150035",strlen("410105199607150035"));
//	driver_info.year = HexToBcd(0x33);
//	driver_info.month = HexToBcd(0x12);
//	driver_info.day = HexToBcd(0x12);
//	driver_info.XOR_word = BccCheckSum((u8*)&driver_info, sizeof(driver_info));
//	AT24CXX_Write(0,(u8*)&driver_info,sizeof(driver_info));
	
//	AT24CXX_Read(0,(u8*)&driver_info,sizeof(driver_info));
//	
//	printf("start \r\n");
//	printf("driver_info.driver_num: %s \r\n",(driver_info.driver_num));
//	printf("driver_info.year: 0x%02x \r\n",BcdToHex(driver_info.year));
//	printf("driver_info.month: 0x%02x \r\n",BcdToHex(driver_info.month));
//	printf("driver_info.day: 0x%02x \r\n",BcdToHex(driver_info.day));
//	printf("driver_info.XOR_word: 0x%02x \r\n",driver_info.XOR_word);

	while(1){}
	
	
	return 0;
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
