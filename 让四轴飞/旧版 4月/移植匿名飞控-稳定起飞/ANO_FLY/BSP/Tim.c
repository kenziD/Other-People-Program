///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ĩ�μ��˱���������ɳ����������˼�Ļ��ͨ���������ʶ�������ø��ܶ������������������Ŷӣ�һ�����о����ᣡ
//�������������˼���û�У���˳������ֲ�͵��Զ����Լ�������ʱ���������ģ�Ҳ�������ʡ�
//������EEPW�����̳�Խ��Խ��������ڷ��׳��Լ��Ĵ��룬���Ǹ��õ�����
//��Ϊֻ�в��Ͻ���������ȡ�ø���Ľ�����������Ƥ�������������~ 
//    
//�����������Ҿ������Լ���ֲ���ԵĴ��뿪Դ�����������ѧϰ��ϣ�������һ�����������������������ǡ�
//��Ϊ���ǵ��׼�����һ���ģ�Ӳ������һ�µ��ˣ��Ǿ�ֻ��Ҫ�о�����ϵ��㷨�����ˡ����������᷽��Ķ࣡
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����DIY����飺http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//������̳��http://forum.eepw.com.cn/forum/368/1 
//
//�ҵ�����DIY��������http://forum.eepw.com.cn/thread/248747/1
//
//�Ա����̣�http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:������ɣ�2014,4,30
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "tim.h"

u32 TIM3_IRQCNT = 0;
/**************************ʵ�ֺ���********************************************
*����ԭ��:		
*��������:1ms�ж�һ��,������Ϊ1000		
*******************************************************************************/
void Tim3_Init(u16 period_num)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//�������ã�ʱ���ͱȽ�������ã���������ֻ�趨ʱ�����Բ���OC�Ƚ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_DeInit(TIM3);

	TIM_TimeBaseStructure.TIM_Period=period_num;//װ��ֵ
	//prescaler is 1200,that is 72000000/72/500=2000Hz;
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;//��Ƶϵ��
	//set clock division 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //or TIM_CKD_DIV2 or TIM_CKD_DIV4
	//count up
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	//clear the TIM2 overflow interrupt flag
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	//TIM2 overflow interrupt enable
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//enable TIM2
	TIM_Cmd(TIM3,ENABLE);
}

void delay_ms(u16 nms)
{	
	TIM3->CNT = 0;
  	while(nms--)
	{
		while(TIM_GetITStatus(TIM3 , TIM_IT_Update) == RESET);
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);  
	}   
}   





