///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//上周末参加了北京的四轴沙龙，很有意思的活动。通过活动，我意识到，想让跟很多的人来加入我们这个团队，一起来研究四轴！
//我身边玩四轴的人几乎没有，因此程序的移植和调试都是自己做，有时遇到不懂的，也不方便问。
//现在在EEPW这个论坛里，越来越多的人乐于奉献出自己的代码，这是个好的现象，
//因为只有不断交流，才能取得更大的进步！三个臭皮匠，赛过诸葛亮~ 
//    
//考虑再三，我决定把自己移植调试的代码开源出来，供大家学习。希望大家能一起，来加入这个活动，来加入我们。
//因为咱们的套件都是一样的，硬件上是一致的了，那就只需要研究软件上的算法就行了。交流起来会方便的多！
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//四轴DIY活动详情：http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//四轴论坛：http://forum.eepw.com.cn/forum/368/1 
//
//我的四轴DIY进程贴：http://forum.eepw.com.cn/thread/248747/1
//
//淘宝店铺：http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:让四轴飞，2014,4,30
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "tim.h"

u32 TIM3_IRQCNT = 0;
/**************************实现函数********************************************
*函数原型:		
*功　　能:1ms中断一次,计数器为1000		
*******************************************************************************/
void Tim3_Init(u16 period_num)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//基础设置，时基和比较输出设置，由于这里只需定时，所以不用OC比较输出
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_DeInit(TIM3);

	TIM_TimeBaseStructure.TIM_Period=period_num;//装载值
	//prescaler is 1200,that is 72000000/72/500=2000Hz;
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;//分频系数
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





