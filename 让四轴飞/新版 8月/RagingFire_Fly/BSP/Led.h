#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"
#include "tim3.h"		

#define LED2_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_3);
#define LED2_ON 			GPIO_ResetBits(GPIOB, GPIO_Pin_3);

#define LED3_ON  		GPIO_SetBits(GPIOC, GPIO_Pin_13);
#define LED3_OFF 			GPIO_ResetBits(GPIOC, GPIO_Pin_13);

void LED_INIT(void);
void LED_FLASH(void);
void LED3_ONOFF(void);

#endif
