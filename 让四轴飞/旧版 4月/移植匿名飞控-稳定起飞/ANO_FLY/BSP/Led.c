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
	//ʹ��LED�õ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //������B��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); //������C��ʱ��
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		 //�ر�JTAG-DP,����SW-DP���ͷ�PB3��PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//�ر�JTAG-DP,����SW-DP���ͷ�PB3��PB4
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	//PB3 LED2 ͨѶָʾ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PC13 LED3 ҹ��ָʾ
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

