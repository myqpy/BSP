#include "sys.h"
#include "terminal_parameter.h"

#define FLASH_GPS_ADDR (uint32_t)0x08034800

void bsp_init(void);
void Input_process(void);
void Reception3399(void);
void Parse3399(u8* USART3_RX_BUF,u16 USART3_RX_STA);


//void statusReport(unsigned char statusBit, unsigned char statusValue);
void system_reboot(void);
void GPS_FLASH_WRITE(void);
void ICcardRead(void);
int packagingMessage(unsigned char msg_id);
int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id);
void update_status(unsigned char statusbit, unsigned char value);
int sendMessage(unsigned char msg_id);
void ICcardWrite(uint8_t writeOrNah);





void LowVoltage_process(float voltage);
void Brake_process(void);
void LeftSignal_process(void);
void LowBeam_process(void);

uint8_t ACC_OFF_process(void);
void actionEverySecond(void);
void setThreeAndHalfHourFlag(void);
