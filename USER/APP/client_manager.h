#include "sys.h"

#define FLASH_GPS_ADDR (uint32_t)0x08034800

void statusReport(unsigned char statusBit, unsigned char statusValue);
void system_reboot(void);
void GPS_FLASH_WRITE(void);
