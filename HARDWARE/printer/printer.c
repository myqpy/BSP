#include "printer.h"
#include "stdio.h"
#include "string.h"
u8 USART4_RX_BUF[USART4_REC_LEN];
unsigned char out_line[] = {0x1b,0x66,0x01,0x02};//�����������
unsigned char reg_num_china[] = {0xBB,0xFA,0xB6,0xAF,0xB3,0xB5,0xBA,0xC5,0xC5,0xC6,0xA3,0xBA};//"���������ƣ����ֱ���"
u16 USART4_RX_STA=0;       //����״̬���

uint8_t flag=0;

void UART4_send_byte(uint8_t byte)
{
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);//�ȴ��������
    UART4->DR=byte;
}

//���Ͷ��ֽ�����
void printer_send_cmd(uint8_t *Buffer, uint16_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        UART4_send_byte(Buffer[i++]);
    }
}
void printer_init(u32 bound) {
    //GPIO�˿�����
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

    //Usart4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�
    USART_Cmd(UART4, ENABLE);
}

void UART4_IRQHandler(void)
{
    static uint8_t rebuf[13]= {0},i=0;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        rebuf[i++]=USART_ReceiveData(UART4);
        if(rebuf[0]!=0x5a)//�ж�֡ͷ
            i=0;
        if((i==2)&&(rebuf[1]!=0x5a))//�ж�֡ͷ
            i=0;
        if(i>4)//��i����ֵ=5ʱ�������ֽڽ�����ϣ����ݳ����ֽڽ������
        {
            if(i==rebuf[3]+5)
            {
                memcpy(USART4_RX_BUF,rebuf,i);
                USART4_RX_STA=1;
                i=0;
            }
        }
        USART_ClearFlag(UART4,USART_FLAG_RXNE);//���жϱ�־
    }
}

uint8_t CHeck(uint8_t *data)
{
    uint8_t sum=0,number=0,i=0;
    number=USART4_RX_BUF[3]+5;
    if(number>42)//�����ϴ�����
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


