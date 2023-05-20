#ifndef PARSER_H_
#define PARSER_H_
#include "terminal_parameter.h"


int parsingMessage(const unsigned char *in, unsigned int in_len);
int frameParse(MCU_Parameters *para);
int handle_kARMGeneralResponse(MCU_Parameters *para);// +3399通用应答.
int handle_kArmOTrecord(MCU_Parameters *para);// +3399下发超时驾驶记录.
int handle_kTimeCorrect(MCU_Parameters *para);// +3399下发时间校准.
int handle_kSelfCheck(MCU_Parameters *para);// +3399下发自检结果.
int handle_kCarInfo(MCU_Parameters *para);// +3399下发车辆信息.
int handle_kForbidTime(MCU_Parameters *para);// +3399下发禁行时段.
int handle_kLocation(MCU_Parameters *para);// +3399下发位置上报.
int handle_kOTwarning(MCU_Parameters *para);// +3399下发超时预警.
int handle_kZeroMileage(MCU_Parameters *para);// +3399下发里程清零.
int handle_kcheckCommand(MCU_Parameters *para);// +3399下发鉴定命令字.


#endif // PARSER_H_
