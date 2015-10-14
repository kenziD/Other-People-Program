#ifndef _LED_H_
#define _LED_H_

#include "tim.h"		

#define LED1_ON  		GPIO_SetBits(GPIOB, GPIO_Pin_10)	//·äÃùÆ÷
#define LED1_OFF 		GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define LED2_OFF  	GPIO_SetBits(GPIOB, GPIO_Pin_11)	//PB11	green
#define LED2_ON 		GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define LED3_OFF  	GPIO_SetBits(GPIOB, GPIO_Pin_12)//PB12	red
#define LED3_ON 		GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define LEDALL_OFF  LED1_OFF;LED2_OFF;LED3_OFF;
#define LEDALL_ON 	LED1_ON;LED2_ON; LED3_ON;

void LED_INIT(void);
void LED_FLASH(void);

void LED1_ONOFF(void);
void LED2_ONOFF(void);
void LED3_ONOFF(void);

void Delay_ms_led(u16 nms);

#endif
