
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//四轴DIY活动详情：http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//四轴论坛：http://forum.eepw.com.cn/forum/368/1 
//
//我的四轴DIY进程贴：http://forum.eepw.com.cn/thread/248747/1
//
//“烈火四轴”淘宝店铺：http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:让四轴飞，2014-8-13
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "BSP.H"
#include "uart/uart1.h"
#include "rc/rc.h"

void SYS_INIT(void)
{
	LED_INIT();	//led，蜂鸣器，按键
	LED_FLASH();
	KEY_Configration();
	
	Tim3_Init(10000);			//10ms中断

	Uart1_Init(115200);
	Spi1_Init();
	Nvic_Init();				//中断初始化
	Nrf24l01_Init(MODEL_RX2,40);//通道40

	if((Nrf24l01_Check()))	Uart1_Put_String("NRF24L01 IS OK !\r\n");
	else Uart1_Put_String("NRF24L01 IS NOT OK !\r\n");

	AD_DMA_Int();
	InitLcd();
}

int main(void)
{
	SYS_INIT_OK=0;
	SYS_INIT();
	SYS_INIT_OK=1;

	while (1)
	{					
		Uart_CheckEvent();	//将串口接收到的数据由2401发出	
		Nrf_Check_Event();	//将2401接收到的数据返回串口		
		
		if(NRF_SEND_RC_FLAG)	//100HZ，用于2401发送控制信号
		{
			NRF_SEND_RC_FLAG = 0;					
			NRF_TxPacket_AP(NRF24L01_TXDATA_RC,32);		//将控制信号发给四轴
			
			Key_Scan();
			if(play)
			{
				play = 0;
				PlayDY();
				PlayPID();
			}
		}
	}
}

//int fputc(int ch, FILE *f)	//为了正常使用printf而添加！不建议使用。
//{
//	USART_SendData(USART1, (u8) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}


