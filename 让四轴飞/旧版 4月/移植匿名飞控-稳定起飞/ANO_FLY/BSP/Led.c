#include "stm32f10x.h"
#include "led.h"

void Delay_ms_led(u16 nms)
{	
	uint16_t i,j;
	for(i=0;i<nms;i++)
		for(j=0;j<8500;j++);
} 

void LED_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能LED用的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //打开外设B的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); //打开外设C的时钟
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		 //关闭JTAG-DP,开启SW-DP，释放PB3，PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//关闭JTAG-DP,开启SW-DP，释放PB3，PB4
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	//PB3 LED2 通讯指示
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PC13 LED3 夜间指示
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//////////////////////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //SCL
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void LED_FLASH(void)
{
	LEDALL_ON;
	Delay_ms_led(200);
	LEDALL_OFF;
	Delay_ms_led(200);
	
	LEDALL_ON;
	Delay_ms_led(200);
	LEDALL_OFF;
	Delay_ms_led(200);

	LEDALL_ON;
	Delay_ms_led(200);
	LEDALL_OFF;
	Delay_ms_led(200);
}

