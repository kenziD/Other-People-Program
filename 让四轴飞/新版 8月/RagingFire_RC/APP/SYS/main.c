
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����DIY����飺http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//������̳��http://forum.eepw.com.cn/forum/368/1 
//
//�ҵ�����DIY��������http://forum.eepw.com.cn/thread/248747/1
//
//���һ����ᡱ�Ա����̣�http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:������ɣ�2014-8-13
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "BSP.H"
#include "uart/uart1.h"
#include "rc/rc.h"

void SYS_INIT(void)
{
	LED_INIT();	//led��������������
	LED_FLASH();
	KEY_Configration();
	
	Tim3_Init(10000);			//10ms�ж�

	Uart1_Init(115200);
	Spi1_Init();
	Nvic_Init();				//�жϳ�ʼ��
	Nrf24l01_Init(MODEL_RX2,40);//ͨ��40

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
		Uart_CheckEvent();	//�����ڽ��յ���������2401����	
		Nrf_Check_Event();	//��2401���յ������ݷ��ش���		
		
		if(NRF_SEND_RC_FLAG)	//100HZ������2401���Ϳ����ź�
		{
			NRF_SEND_RC_FLAG = 0;					
			NRF_TxPacket_AP(NRF24L01_TXDATA_RC,32);		//�������źŷ�������
			
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

//int fputc(int ch, FILE *f)	//Ϊ������ʹ��printf����ӣ�������ʹ�á�
//{
//	USART_SendData(USART1, (u8) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}


