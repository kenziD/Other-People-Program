#ifndef _BSP_USART_H_
#define _BSP_USART_H_

extern u8 Rx_Buf[2][32];	//����32�ֽڵĴ��ڽ��ջ���
extern u8 Rx_Ok0;		//������ϱ�־
extern u8 Rx_Ok1;

void Uart1_Init(u32 br_num);
void Uart1_IRQ(void);
void Uart1_Put_String(unsigned char *Str);
uint8_t Uart1_Put_Char(unsigned char DataToSend);
uint8_t Uart1_Put_Int16(uint16_t DataToSend);
uint8_t Uart1_Put_Float(float DataToSend);

#endif
