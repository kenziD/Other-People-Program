
#include "uart1.h"
#include "rc/rc.h"

extern void NRF_TxPacket_AP(uint8_t * tx_buf, uint8_t len);

void Uart_CheckEvent(void)
{
	if(Rx_Ok0)
	{
		Rx_Ok0 = 0;
		
		NRF_TxPacket_AP(Rx_Buf[0],Rx_Buf[0][3]+5);
	}
	if(Rx_Ok1)
	{
		Rx_Ok1 = 0;

		NRF_TxPacket_AP(Rx_Buf[1],Rx_Buf[0][3]+5);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Uart1_Send_Buf(u8 *buf,u8 len)		//发送buf,长度len,返回字节和sum
{
	while(len)
	{
		Uart1_Put_Char(*buf);
		buf++;
		len--;
	}
}



