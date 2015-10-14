
#include "BSP.H"
#include "rc/rc.h"

void Key_Scan(void)
{
	if(key!=0)
	{
		key--;
	}
	else
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)==0)	//FUN	//1£¬ËøÎ²£»0£¬²»ËøÎ²
		{
			key = 40;
			aux6++;
			if(aux6>1)
				aux6 = 0;
			PutChar(aux6+0x10,0,8);
			Rc_Get.AUX6 = aux6;
		}
//		Rc_Get.AUX6 = 0;
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)==0)	//MODE
//		{
//			key = 40;
//			aux6++;
//			if(aux6>4)
//				aux6 = 1;
//			PutChar(aux6+0x10,0,8);
//		}
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)==0)	//FUN
//		{
//			key = 100;
//			Rc_Get.AUX6 = aux6;
//		}
	}
}

void Delay_ms_led(u16 nms)
{	
	uint16_t i,j;
	for(i=0;i<nms;i++)
		for(j=0;j<8500;j++);
} 

void LED_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //´ò¿ªÍâÉèBµÄÊ±ÖÓ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	//PB10 	·äÃùÆ÷
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 	//PB11	green
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PB12	red
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED_FLASH(void)
{
	LEDALL_ON
	Delay_ms_led(150);
	LEDALL_OFF
	Delay_ms_led(150);
	LEDALL_ON
	Delay_ms_led(150);
	LEDALL_OFF
}

void LED1_ONOFF(void)	//·äÃùÆ÷
{
	LED1_ON;
	Delay_ms_led(50);
	LED1_OFF;
	Delay_ms_led(50);
}

void LED2_ONOFF(void)	//ÂÌµÆ
{
	LED2_ON;
	Delay_ms_led(50);
	LED2_OFF;
	Delay_ms_led(50);
}

void LED3_ONOFF(void)	//ºìµÆ
{
	LED3_ON;
	Delay_ms_led(50);
	LED3_OFF;
	Delay_ms_led(50);
}

