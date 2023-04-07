#ifndef __PRINTER_H
#define __PRINTER_H
#include "stdio.h"	
#include "sys.h" 

#define USART4_REC_LEN  			200  	//定义最大接收字节数 200
#pragma pack(1)
typedef struct{
	unsigned char reg_num_chinese[12];
	unsigned char reg_num[19];	
}printer_info_t;

extern u8  USART4_RX_BUF[USART4_REC_LEN];
extern u16 USART4_RX_STA;

void printer_init(u32 bound);
void printer_send_cmd(uint8_t *Buffer, uint16_t Length);
void UART4_send_byte(uint8_t byte);
uint8_t CHeck(uint8_t *data);
void printer_info_init(u8 *cmd, u8 *reg_num);
void print_empty_line(void);
#endif

