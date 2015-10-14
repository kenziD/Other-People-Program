#include <stm32f10x.h>
#include "delay.h"


void delay_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM4);	                //���½�Timer����Ϊȱʡֵ 
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;	     //1Ϊ1ms		//���ü��������С��ÿ��1000�����Ͳ���һ�������¼�
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	 //Ԥ��Ƶϵ��Ϊ720-1������������ʱ��Ϊ72MHz/7200 = 100kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	  //����ʱ�ӷָ�   0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ü�����ģʽΪ���ϼ���ģʽ		S
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ� 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);	//������Ӧ�õ�TIM4��
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);          //�������жϱ�־
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);	    //����TIM4���ж�

    TIM_Cmd(TIM4,ENABLE);                            //��TIM4S
	
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


















