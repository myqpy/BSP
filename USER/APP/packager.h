#ifndef PACKAGER_H_
#define PACKAGER_H_
#include "terminal_parameter.h"

void bufferSendPushByte(unsigned char byte);

int McuFrameBodyPackage(MCU_Parameters *para);
//int McuFramePackage(MCU_Parameters *para);
int McuMsgPackage(MCU_Parameters *para);
int McuOTPackage(MCU_Parameters *para,unsigned char Page);
int McuSOSPackage(MCU_Parameters *para,unsigned char statusbit, unsigned char value);
int handle_GeneralResponse(MCU_Parameters *para);
int handle_StatusReport(MCU_Parameters *para);
int handle_AlarmReport(unsigned char statusbit, unsigned char value);
int handle_AcquireOTReport(unsigned char Page);
#endif // PACKAGER_H_
