#include "stdio.h"

#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#define N 2
#define M 8 	

extern unsigned char i;
extern unsigned char play;

extern u16 AD_Value[N][M]; 
extern u16 After_filter[M];
extern u16 value[M];
extern u16 dianya0,dianya1;

void ADC_Configuration(void); 
void DMA_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);

void PlayDY(void);
