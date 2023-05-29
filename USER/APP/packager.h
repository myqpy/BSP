#ifndef PACKAGER_H_
#define PACKAGER_H_
#include "terminal_parameter.h"
#define BUFFER_SIZE_SEND 200 // buffer size 发送缓存大小


extern uint8_t McuPackage[BUFFER_SIZE_SEND]; //发送缓存
extern uint32_t RealBufferSendSize;

void bufferSendPushByte(unsigned char byte);
int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id);
int McuFrameBodyPackage(MCU_Parameters *para);
int McuMsgPackage(MCU_Parameters *para);
int handle_GeneralResponse(MCU_Parameters *para);
int handle_StatusReport(MCU_Parameters *para);
int handle_AlarmReport(MCU_Parameters *para);
int handle_AcquireOTReport(MCU_Parameters *para);
int handle_MCUCANReport(MCU_Parameters *para);
#endif // PACKAGER_H_
