#ifndef PARSER_H_
#define PARSER_H_
#include "terminal_parameter.h"


int parsingMessage(const unsigned char *in, unsigned int in_len);
int frameParse(MCU_Parameters *para);
int handle_kARMGeneralResponse(MCU_Parameters *para);// +3399ͨ��Ӧ��.
int handle_kArmOTrecord(MCU_Parameters *para);// +3399�·���ʱ��ʻ��¼.
int handle_kTimeCorrect(MCU_Parameters *para);// +3399�·�ʱ��У׼.
int handle_kSelfCheck(MCU_Parameters *para);// +3399�·��Լ���.
int handle_kCarInfo(MCU_Parameters *para);// +3399�·�������Ϣ.
int handle_kForbidTime(MCU_Parameters *para);// +3399�·�����ʱ��.
int handle_kLocation(MCU_Parameters *para);// +3399�·�λ���ϱ�.
int handle_kOTwarning(MCU_Parameters *para);// +3399�·���ʱԤ��.
int handle_kZeroMileage(MCU_Parameters *para);// +3399�·��������.
int handle_kcheckCommand(MCU_Parameters *para);// +3399�·�����������.


#endif // PARSER_H_
