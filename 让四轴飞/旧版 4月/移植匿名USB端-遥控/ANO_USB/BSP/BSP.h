#ifndef _BSP_BSP_H_
#define _BSP_BSP_H_
#include "tim.h"
#include "BSP/usart.h"
#include "spi.h"
#include "nrf24l01.h"

extern u8 SYS_INIT_OK;

void Nvic_Init(void);

#endif
