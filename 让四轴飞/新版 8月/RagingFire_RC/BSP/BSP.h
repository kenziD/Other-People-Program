#ifndef _BSP_BSP_H_
#define _BSP_BSP_H_
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "nrf24l01.h"
#include "DMA_ADC.h"
#include "led.h"
#include "5110/LCD5110.h"

extern u8 SYS_INIT_OK;
extern volatile u8 play,key,aux6;

void Nvic_Init(void);
void KEY_Configration(void);
void Key_Scan(void);

#endif
