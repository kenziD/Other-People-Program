#ifndef _BSP_USART_H_
#define _BSP_USART_H_

extern u8 Rx_Buf[2][32];	//两个32字节的串口接收缓存
extern u8 Rx_Ok0;		//接收完毕标志
extern u8 Rx_Ok1;

void Uart1_Init(u32 br_num);
void Uart1_IRQ(void);
void Uart1_Put_String(unsigned char *Str);
uint8_t Uart1_Put_Char(unsigned char DataToSend);
uint8_t Uart1_Put_Int16(uint16_t DataToSend);
uint8_t Uart1_Put_Float(float DataToSend);

// void PC_LED(uint8_t num,uint8_t sta);//sta=0 熄灭 sta=1 点亮  >1 取反

#endif
