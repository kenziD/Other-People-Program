#ifndef __DELAY_H
#define __DELAY_H 			   
#include <stm32f10x.h>
//Mini STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms

//V1.2
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void Delay(vu32 nCount);
#endif





























