#include "led.h"

void Delay_ms_led(u16 nms)
{	
	uint16_t i,j;
	for(i=0;i<nms;i++)
		for(j=0;j<8500;j++);
} 

void LED_INIT(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_Structure.GPIO_Pin =  GPIO_Pin_3;	//Ö¸Ê¾µÆ
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  GPIO_Pin_13;	//Ò¹¼äµÆ
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_Structure);
}

void LED_FLASH(void)
{
	LED2_ON;
	Delay_ms_led(100);
	LED3_ON;
	Delay_ms_led(100);
	LED2_OFF;
	LED3_OFF;
	Delay_ms_led(100);
}
void LED3_ONOFF(void)
{
	static uint8_t busy=0;
	static uint8_t led3_sta=1;
	static uint32_t time_temp;
	if(led3_sta)
	{
		if(!busy)
		{
			time_temp = TIM3_IRQCNT;
			busy=1;
		}
		else if(time_temp < TIM3_IRQCNT)
		{
			led3_sta=0;
			LED3_OFF;
			busy=0;
		}
	}
	else
	{
		if(!busy)
		{
			time_temp = TIM3_IRQCNT;
			busy=1;
		}
		else if(time_temp < TIM3_IRQCNT)
		{
			led3_sta=1;
			LED3_ON;
			busy=0;
		}
	}
}

