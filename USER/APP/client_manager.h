#include "sys.h"
#include "terminal_parameter.h"

#define FLASH_GPS_ADDR (uint32_t)0x08034800

//void statusReport(unsigned char statusBit, unsigned char statusValue);
void system_reboot(void);
void GPS_FLASH_WRITE(void);
void ICcardRead(void);
int packagingMessage(unsigned char msg_id);
int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id);
void update_status(unsigned char statusbit, unsigned char value);
int sendMessage(unsigned char msg_id);
void ICcardWrite(uint8_t writeOrNah);
