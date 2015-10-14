
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

u8 SYS_INIT_OK = 0;

void KEY_Configration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//MODE
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//FUN
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
   
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
//    EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line9;
//    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
}

void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* NVIC_PriorityGroup 2 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//先占优先级0~3，越小优先级越大
	//次占优先级0~3
	//串口
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//TIM3
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//	//MODE
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	//FUN
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}
