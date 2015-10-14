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
#include "uart1.h"
#include "app/rc/rc.h"

void Uart_DataAnl(u8 buf_num)		//串口缓存数据分析
{
	if(Rx_Buf[buf_num][0]==0x8A)		//串口收到的是上位机发给飞机的数据ADC1,直接转发
	{
		NRF_SendData_AP(Rx_Buf[buf_num]);
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
	sum += Uart1_Put_Char(0xAE);	//遥控,电机pwm,电压显示对应的帧FUN为0xAE
	sum += Uart1_Put_Char(28);
	sum += Uart1_Put_Int16(Rc_Get.THROTTLE);
	sum += Uart1_Put_Int16(Rc_Get.YAW);
	sum += Uart1_Put_Int16(Rc_Get.ROLL);
	sum += Uart1_Put_Int16(Rc_Get.PITCH);
	sum += Uart1_Put_Int16(Rc_Get.AUX1);
	sum += Uart1_Put_Int16(Rc_Get.AUX2);
	sum += Uart1_Put_Int16(Rc_Get.AUX3);
	sum += Uart1_Put_Int16(Rc_Get.AUX4);
	sum += Uart1_Put_Int16(Rc_Get.AUX5);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	sum += Uart1_Put_Int16(0);
	Uart1_Put_Char(sum);
}

void PC_Debug_Show(u8 num,u16 sta)//sta=0 熄灭 sta=1 点亮  >1 取反
{
	static uint8_t led_s[6] = {0,0,0,0,0,0};
	uint8_t sum = 0;
	if(0<num && num<7)
	{
		sum += Uart1_Put_Char(0x88);
		sum += Uart1_Put_Char(0xAD);
		sum += Uart1_Put_Char(0x02);
		sum += Uart1_Put_Char(num);
		if(sta==0)
			sum += Uart1_Put_Char(0x00);
		else if(sta==1)
			sum += Uart1_Put_Char(0x01);
		else 
		{
			if(led_s[num])	led_s[num] = 0;
			else 			led_s[num] = 1;
			sum += Uart1_Put_Char(led_s[num]);
		}
		Uart1_Put_Char(sum);
	}
	else if(6<num && num<13)
	{
		sum += Uart1_Put_Char(0x88);
		sum += Uart1_Put_Char(0xAD);
		sum += Uart1_Put_Char(0x03);
		sum += Uart1_Put_Char(num);
		sum += Uart1_Put_Int16(sta);
		Uart1_Put_Char(sum);
	}
}
