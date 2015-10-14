#include <stm32f10x.h>
#include "delay.h"


void delay_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM4);	                //重新将Timer设置为缺省值 
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;	     //1为1ms		//设置计数溢出大小，每计1000个数就产生一个更新事件
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	 //预分频系数为720-1，这样计数器时钟为72MHz/7200 = 100kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	  //设置时钟分割   0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //设置计数器模式为向上计数模式		S
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);	//将配置应用到TIM4中
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);          //清除溢出中断标志
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);	    //开启TIM4的中断

    TIM_Cmd(TIM4,ENABLE);                            //打开TIM4S
	
}								    

void delay_ms(u16 nms)
{	
	TIM4->CNT = 0;
  	while(nms--)
	{
		while(TIM_GetITStatus(TIM4 , TIM_IT_Update) == RESET);
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);  
	}   
}   
		    								   
void delay_us(u32 nus)
{	
	TIM4->CNT = 1000 - nus;
	while(TIM_GetITStatus(TIM4 , TIM_IT_Update) == RESET);
	TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 	 
}
#if 1
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
#endif


















