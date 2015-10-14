/**
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
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp/bsp.h"
#include "app/uart/uart1.h"
#include "app/rc/rc.h"

#include "DMA_ADC.h"
#include "led.h"
#include "LCD5110.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
u8 aa;
u16 dianya0 = 0,dianya1 = 0;
u16 qh=0,zy=0,ph=0;
unsigned char play;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
/*=====================================================================================================*/
/*=====================================================================================================*/
void USART1_IRQHandler(void)  //串口中断函数
{
	Uart1_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
////////////void TIM2_IRQHandler(void)		//用于捕捉PPM,使用PA0
////////////{
//////////////	static u8 	RC_Ch = 0;
//////////////	static u16 	RC_Value[10];
////////////	
////////////	if(TIM2->SR & TIM_IT_CC1)//if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
////////////	{
////////////		u16 CC1_Value_Now = TIM2->CCR1;//读ccr,清cc1if标志
////////////		TIM2->CNT = 0;				//清cnt
////////////		//这里不清除标志位，则无法正常运行！！！
////////////			
//////////////			if(CC1_Value_Now>3000)	//判断同步信号	//这部分是捕抓ppm的，不用则屏蔽
//////////////				RC_Ch = 0;
//////////////			RC_Value[RC_Ch] = CC1_Value_Now;
//////////////			RC_Ch++;
//////////////			
//////////////			if(RC_Ch==1)	//	不使用ppm，此部分屏蔽，用于ad采集遥控信号后发送
//////////////			{
//////////////				Rc_Get.THROTTLE = RC_Value[3];
//////////////				Rc_Get.YAW = RC_Value[4];
//////////////				Rc_Get.ROLL = RC_Value[1];
//////////////				Rc_Get.PITCH = RC_Value[2];
//////////////				Rc_Get.AUX1 = RC_Value[5];
//////////////				Rc_Get.AUX2 = RC_Value[6];
//////////////				Rc_Get.AUX3 = RC_Value[7];
//////////////				Rc_Get.AUX4 = RC_Value[8];
//////////////				Rc_Get.AUX5 = RC_Value[9];
////////////////				//Uart1_Send_RCdata();
//////////////				NRF_Send_RC();
//////////////			}
////////////	}	
////////////}
/*=====================================================================================================*/
/*=====================================================================================================*/

void TIM3_IRQHandler(void)		//10ms中断一次
{	
	static u8 ms20 = 0,ms500 = 0;				//中断次数计数器
	if(TIM3->SR & TIM_IT_Update)		//if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{     
		TIM3->SR = ~TIM_FLAG_Update;  //TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //清除中断标志
		if(!SYS_INIT_OK)		
			return;
		//每次中断都执行,10ms
		ms20++;

		if(ms20==2)	  //每2次中断执行一次,20ms	//该处用于50HZ遥控信号
		{
			ms20=0;	
			ms500++;
			if(ms500==25)	//2HZ
			{
				ms500=0;
				LEDALL_OFF
				
				play = 1;	 //屏幕显示
				if(play)
				{
					play = 0;
					PlayDY();
//					switch (aa%2)
//					{
//						case 0:	bl0;	break;
//						case 1:	bl1;	break;
//					}
				}
				
				if(dianya1 < 37 && dianya1 >20)		//遥控电压
				{
					LED1_ON;
					LED2_ON;	//绿
				}
				if(dianya0 < 35 && dianya0 > 20)	//四轴电压，2200，3.75左右	//为了保险起见，可以将35适当调大至36
				{
					LED1_ON;
					LED3_ON;	//红
				}				
			}
		
			Rc_Get.THROTTLE = (u16)(0.25*AD_Value[0][4])+1000;	
			Rc_Get.YAW = (u16)(0.74*AD_Value[0][5]);		//偏航角
			Rc_Get.ROLL = 3000 - (u16)(0.73*AD_Value[0][7])-11;	//横滚角
			Rc_Get.PITCH =(u16)(0.732*AD_Value[0][6]);//俯仰角	
			
			Rc_Get.AUX1 = (u16)(AD_Value[0][0]/80); //左右微调
			Rc_Get.AUX2 = (u16)(AD_Value[0][1]/80);	//旋转微调
			Rc_Get.AUX3 = (u16)(AD_Value[0][2]/80); //前后微调
			Rc_Get.AUX4 = (u16)(AD_Value[0][3]*6.6/410);	//电压值的十倍		
					dianya1 = (u16)Rc_Get.AUX4;
			Rc_Get.AUX5 = ((u16)(NRF24L01_RXDATA[19])) << 8 | NRF24L01_RXDATA[20];	
					dianya0 =	(u16)((Rc_Get.AUX5)*6.6/410);

			NRF_Send_RC();	//准备要发送的遥控命令
											//装入数组NRF24L01_TXDATA_RC
											//使NRF_SEND_RC_FLAG=1					
		}
		
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
// void EXTI1_IRQHandler(void)
// {
// 	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
// 	{
// 		EXTI_ClearITPendingBit(EXTI_Line1);
// 	}
// }
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
