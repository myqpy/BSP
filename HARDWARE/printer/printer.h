#ifndef __PRINTER_H
#define __PRINTER_H
#include "stdio.h"	
#include "sys.h" 

#define USART4_REC_LEN  			200  	//定义最大接收字节数 200


extern u8  USART4_RX_BUF[USART4_REC_LEN];
extern u16 USART4_RX_STA;
void UART4_send_byte(uint8_t byte);
void printer_send_cmd(char *Buffer, uint16_t Length);

void UART4_init(u32 bound);
uint8_t CHeck(uint8_t *data);

#endif

