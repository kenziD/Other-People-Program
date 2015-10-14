#include "uart1.h"
#include "app/rc/rc.h"

void Uart_DataAnl(u8 buf_num)		//串口缓存数据分析
{
	if(Rx_Buf[buf_num][1]==0x8A)		//串口收到的是上位机的遥控数据
	{
		Uart1_Put_Char(0x30+buf_num);
	}
}
void Uart_CheckEvent(void)
{
	if(Rx_Ok0)
	{
		Rx_Ok0 = 0;
		u8 sum = 0;
		for(int i=0;i<31;i++)
			sum += Rx_Buf[0][i];
		if(sum == Rx_Buf[0][31])		//和校验通过
		{
			Uart_DataAnl(0);
		}
	}
	if(Rx_Ok1)
	{
		Rx_Ok1 = 0;
		u8 sum = 0;
		for(int i=0;i<31;i++)
			sum += Rx_Buf[1][i];
		if(sum == Rx_Buf[1][31])		//和校验通过
		{
			Uart_DataAnl(1);
		}
	}
}

void Uart1_Send_Buf(u8 *buf,u8 len)		//发送buf,长度len,返回字节和sum
{
	while(len)
	{
		Uart1_Put_Char(*buf);
		buf++;
		len--;
	}
}
void Uart1_Send_RCdata(void)
{
	uint8_t sum = 0;
	sum += Uart1_Put_Char(0x88);
	sum += Uart1_Put_Char(0xAE);
	sum += Uart1_Put_Char(28);
	sum += Uart1_Put_Int16(Rc_Get.THROTTLE);
	sum += Uart1_Put_Int16(Rc_Get.YAW);
	sum += Uart1_Put_Int16(Rc_Get.ROLL);
	sum += Uart1_Put_Int16(Rc_Get.PITCH);
	sum += Uart1_Put_Int16(Rc_Get.AUX1);
	sum += Uart1_Put_Int16(Rc_Get.AUX2);
	sum += Uart1_Put_Int16(Rc_Get.AUX3);
	sum += Uart1_Put_Int16(Rc_Get.AUX4);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	Uart1_Put_Char(sum);
}


