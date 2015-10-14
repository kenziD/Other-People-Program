
/******************************************************************************
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
#include "bsp.h"
#include "rc/rc.h"
#include "uart/uart1.h"
#include "stm32f10x_exti.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
 **/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 dianya0 = 0,dianya1 = 0;//四轴与遥控电压
volatile u8 play,key,aux6;
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

void TIM3_IRQHandler(void)		//10ms中断一次
{	
	static u8 ms10 = 0,ms200 = 0;		//中断次数计数器
	if(TIM3->SR & TIM_IT_Update)		//if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{     
		TIM3->SR = ~TIM_FLAG_Update;  	//TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //清除中断标志
		if(!SYS_INIT_OK)		
			return;
		
		ms10++;	//每次中断都执行,10ms

		if(ms10==1)	  //每1次中断执行一次,10ms	//该处用于100HZ遥控信号
		{
			ms10=0;	
			ms200++;
			if(ms200==20)	//5HZ
			{
				ms200=0;
				play=1;
			}

			Rc_Get.THROTTLE = 	(u16)(0.25*AD_Value[0][4])+1000;//油门
			Rc_Get.YAW 		= 	(u16)(0.74*AD_Value[0][5]);		//偏航角
			Rc_Get.PITCH 	= 	(u16)(0.732*AD_Value[0][7]);	//俯仰角
			Rc_Get.ROLL 	=	(u16)(0.732*AD_Value[0][6]);	//横滚角
			
			Rc_Get.AUX1 = 	(u16)(AD_Value[0][2]/4); 		//p参数	放大 100倍进行传输
			Rc_Get.AUX2 = 	(u16)(AD_Value[0][0]/40);		//i参数	放大1000倍进行传输
			Rc_Get.AUX3 = 	(u16)(AD_Value[0][1]/160); 		//d参数	放大 100倍进行传输
			Rc_Get.AUX4 = 	(u16)(AD_Value[0][3]*2*33/4096);//遥控电压值的十倍

				dianya1 = 	(u16)Rc_Get.AUX4;

			NRF_Send_RC();	//准备要发送的遥控命令
							//装入数组NRF24L01_TXDATA_RC
							//使NRF_SEND_RC_FLAG=1
		}
		
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
//void EXTI1_IRQHandler(void)	//MODE
//{
//	Delay_ms_led(2);
//	
//	if(EXTI_GetITStatus(EXTI_Line1) != RESET) 
//	{
//		EXTI_ClearITPendingBit(EXTI_Line1); 
//		aux6++;
//		if(aux6>4)
//			aux6 = 0;
//		PutChar(aux6+0x10,0,8);
//	}
//}
///*=====================================================================================================*/
///*=====================================================================================================*/
//void EXTI9_5_IRQHandler(void)	//FUN
//{
//	Delay_ms_led(2);
//	
//	if(EXTI_GetITStatus(EXTI_Line9) != RESET) 
//	{
//		EXTI_ClearITPendingBit(EXTI_Line9);
//		Rc_Get.AUX6 = aux6;
//	}
//}
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
