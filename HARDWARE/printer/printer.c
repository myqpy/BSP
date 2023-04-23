#include "printer.h"
#include "stdio.h"
#include "string.h"
u8 USART4_RX_BUF[USART4_REC_LEN];
unsigned char out_line[] = {0x1b,0x66,0x01,0x02};//输出两个空行
unsigned char reg_num_china[] = {0xBB,0xFA,0xB6,0xAF,0xB3,0xB5,0xBA,0xC5,0xC5,0xC6,0xA3,0xBA};//"机动车号牌：汉字编码"
u16 USART4_RX_STA=0;       //接收状态标记

uint8_t flag=0;

void UART4_send_byte(uint8_t byte)
{
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);//等待发送完成
    UART4->DR=byte;
}

//发送多字节数据
void printer_send_cmd(uint8_t *Buffer, uint16_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        UART4_send_byte(Buffer[i++]);
    }
}
void printer_init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    //USART4_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART4_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //Usart4 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启接收中断
    USART_Cmd(UART4, ENABLE);
}

void UART4_IRQHandler(void)
{
    static uint8_t rebuf[13]= {0},i=0;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        rebuf[i++]=USART_ReceiveData(UART4);
        if(rebuf[0]!=0x5a)//判断帧头
            i=0;
        if((i==2)&&(rebuf[1]!=0x5a))//判断帧头
            i=0;
        if(i>4)//当i计数值=5时，功能字节接受完毕，数据长度字节接收完毕
        {
            if(i==rebuf[3]+5)
            {
                memcpy(USART4_RX_BUF,rebuf,i);
                USART4_RX_STA=1;
                i=0;
            }
        }
        USART_ClearFlag(UART4,USART_FLAG_RXNE);//清中断标志
    }
}

uint8_t CHeck(uint8_t *data)
{
    uint8_t sum=0,number=0,i=0;
    number=USART4_RX_BUF[3]+5;
    if(number>42)//超过上传数据
        return 0;
    for(i=0; i<number-1; i++)
        sum+=USART4_RX_BUF[i];
    if(sum==USART4_RX_BUF[number-1])
    {
        memcpy(data,USART4_RX_BUF,number);
        return 1;
    }
    else
        return 0;
}
void printer_info_init(u8 *cmd, u8 *reg_num)
{
    printer_info_t *send_cmd = (printer_info_t*)cmd;
    memcpy(send_cmd->reg_num_chinese, reg_num_china, sizeof(reg_num_china));
    memset(send_cmd->reg_num, 0x0d, 19);
    memcpy(send_cmd->reg_num, reg_num, 19);
}
void print_empty_line()
{
    printer_send_cmd(out_line, sizeof(out_line));
}


