#ifndef _LED_H_
#define _LED_H_

#include "tim.h"		

#define LED2_OFF  	GPIO_SetBits(GPIOB, GPIO_Pin_3)	//PB3 LED2 通讯指示
#define LED2_ON 		GPIO_ResetBits(GPIOB, GPIO_Pin_3)

#define LED3_ON  	GPIO_SetBits(GPIOC, GPIO_Pin_13)//PC13 LED3 夜间指示
#define LED3_OFF 		GPIO_ResetBits(GPIOC, GPIO_Pin_13)

#define LEDALL_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_3 );LED3_OFF;
#define LEDALL_ON 	GPIO_ResetBits(GPIOB, GPIO_Pin_3 );LED3_ON;

void LED_INIT(void);
void LED_FLASH(void);

void Delay_ms_led(u16 nms);

#endif
