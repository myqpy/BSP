#include "terminal_parameter.h"
#include "client_manager.h"
#include <string.h>
#include "usart.h"
#include "usart3.h"

u8 statusCmd[3];

void system_reboot(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}


void statusReport(unsigned char statusBit, unsigned char statusValue)
{
	statusCmd[0]= 0xDD;
	statusCmd[1]= statusBit;
	statusCmd[2]= statusValue;

	Usart_SendStr_length(USART3, statusCmd, 3);
}
