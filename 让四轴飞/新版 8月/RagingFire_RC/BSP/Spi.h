#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f10x.h"

#define SPI_CE_H()   GPIO_SetBits(GPIOC, GPIO_Pin_14) 
#define SPI_CE_L()   GPIO_ResetBits(GPIOC, GPIO_Pin_14)

#define SPI_CSN_H()  GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define SPI_CSN_L()  GPIO_ResetBits(GPIOC, GPIO_Pin_13)

void Spi1_Init(void);
u8 Spi_RW(u8 dat);

#endif
