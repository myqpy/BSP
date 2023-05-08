#include "sys.h"
#include "terminal_parameter.h"

#define FLASH_GPS_ADDR (uint32_t)0x08034800

//void statusReport(unsigned char statusBit, unsigned char statusValue);
void system_reboot(void);
void GPS_FLASH_WRITE(void);
void ICcardRead(MCU_ICcard_info *driver_info);
int packagingMessage(unsigned int msg_id);
int packagingSOSMessage(unsigned char statusbit, unsigned char value);
int packagingOTMessage(unsigned char Page);
int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id);
