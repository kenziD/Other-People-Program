#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"

#define u32 unsigned int

#define SYSTEMFREQ 8000000

void Delayus(u32 us); 
void Delayms(u32 ms);

	

#endif
