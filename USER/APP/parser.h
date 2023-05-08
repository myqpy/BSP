#ifndef PARSER_H_
#define PARSER_H_
#include "terminal_parameter.h"


int parsingMessage(const unsigned char *in, unsigned int in_len);
int frameParse(MCU_Parameters *para);
int handle_kARMGeneralResponse(MCU_Parameters *para);
int handle_kArmOTrecord(MCU_Parameters *para);
int handle_kTimeCorrect(MCU_Parameters *para);
int handle_kSelfCheck(MCU_Parameters *para);
int handle_kCarInfo(MCU_Parameters *para);
int handle_kForbidTime(MCU_Parameters *para);
int handle_kLocation(MCU_Parameters *para);
int handle_kOTwarning(MCU_Parameters *para);


#endif // PARSER_H_
