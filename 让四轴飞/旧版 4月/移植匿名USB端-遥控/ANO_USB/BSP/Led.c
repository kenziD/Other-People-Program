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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	//������
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void LED_FLASH(void)
{
	LEDALL_ON;
	Delay_ms_led(150);
	LEDALL_OFF;
	Delay_ms_led(150);
	LEDALL_ON;
	Delay_ms_led(150);
	LEDALL_OFF;
}

void LED1_ONOFF(void)	//������
{
	LED1_ON;
	Delay_ms_led(50);
	LED1_OFF;
	Delay_ms_led(50);
}

void LED2_ONOFF(void)	//�̵�
{
	LED2_ON;
	Delay_ms_led(50);
	LED2_OFF;
	Delay_ms_led(50);
}

void LED3_ONOFF(void)	//���
{
	LED3_ON;
	Delay_ms_led(50);
	LED3_OFF;
	Delay_ms_led(50);
}

