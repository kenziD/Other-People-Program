#ifndef _BSP_TIM_H_
#define _BSP_TIM_H_
#include "stm32f10x.h"

extern u32 TIM3_IRQCNT;

void Tim3_Init(u16 period_num);//���ھ�ȷ��ʱ

void delay_ms(u16 nms);

#endif
