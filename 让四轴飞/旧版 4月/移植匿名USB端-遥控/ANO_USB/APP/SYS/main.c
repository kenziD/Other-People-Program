﻿///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "BSP/BSP.H"
#include "app/uart/uart1.h"
#include "app/rc/rc.h"

#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)
////////////////////////////////////////////////////////////////////////////////
//									以下是我添加的
////////////////////////////////////////////////////////////////////////////////
#include "DMA_ADC.h"
#include "led.h"
#include "LCD5110.h"

/////////////////////////////////////////////////////////////////////////////////
//									以上是我添加的
/////////////////////////////////////////////////////////////////////////////////

void SYS_INIT(void)
{
	LED_INIT();
	LED_FLASH();
	
	Tim3_Init(10000);			//1000=1MS,500=0.5MS //10ms中断
//	Tim2_Init();					//用于捕捉PPM,这里我没用到
	Uart1_Init(115200);		
	Spi1_Init();
	Nvic_Init();					//中断初始化
	Nrf24l01_Init(MODEL_RX2,40);//通道40

	if((Nrf24l01_Check()))	Uart1_Put_String("NRF24L01 IS OK !\r\n");
	else Uart1_Put_String("NRF24L01 IS NOT OK !\r\n");
	
////////////////////////////////////////////////////////////////////////////////
//									以下是我添加的
////////////////////////////////////////////////////////////////////////////////
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	DMA_Configuration();
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//启动 ADC1 转换 
	DMA_Cmd(DMA1_Channel1, ENABLE);         //启动 DMA 通道
////////////////////////////////////////////////////////////////////////////////
//									以上是我添加的
////////////////////////////////////////////////////////////////////////////////

}

int main(void)
{
	SYS_INIT_OK=0;
	SYS_INIT();
	SYS_INIT_OK=1;
	
	InitLcd();

	while (1)
	{					
			Uart_CheckEvent();	//将串口接收到的数据由2401发出	//该句可屏蔽，仅用于PC端调试
			Nrf_Check_Event();	//将2401接收到的数据返回串口		//该句不可屏蔽
		
		if(NRF_SEND_RC_FLAG)	//50HZ，用于2401发送控制信号
		{
			NRF_SEND_RC_FLAG = 0;
				
//		  Uart1_Send_RCdata();	//将控制信号回传PC	//该句可屏蔽，仅用于PC端调试
					
			NRF_TxPacket_AP(NRF24L01_TXDATA_RC,32);		//将控制信号发给四轴
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//									以下是我添加的
////////////////////////////////////////////////////////////////////////////////

int fputc(int ch, FILE *f)	//为了正常使用printf而添加！
{
	USART_SendData(USART1, (u8) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}


