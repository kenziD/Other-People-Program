
#ifndef _UART_UART_H_
#define _UART_UART_H_
#include "stm32f10x.h"
#include "usart.h"

void Uart_CheckEvent(void);
void Uart1_Send_Buf(u8 *buf,u8 num);
void Uart1_Send_RCdata(void);

#endif
