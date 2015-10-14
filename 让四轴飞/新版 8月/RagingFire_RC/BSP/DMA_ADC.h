#include "stdio.h"

#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#define N 1
#define M 8

extern u16 AD_Value[N][M]; 
extern u16 dianya0,dianya1;

void AD_DMA_Int(void);

void PlayDY(void);
void PlayPID(void);
void shuzi(u16 value,u8 column,u8 row);
