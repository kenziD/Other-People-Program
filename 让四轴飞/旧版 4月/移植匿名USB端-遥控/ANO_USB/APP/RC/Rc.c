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
#include "rc.h"
#include "bsp/nrf24l01.h"
#include "app/uart/uart1.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define RX_DR			6		//中断标志
#define TX_DS			5
#define MAX_RT		4

RC_GETDATA Rc_Get;//接收到的RC数据,1000~2000
u8 NRF_SEND_RC_FLAG = 0;

void NRF_SendData_AP(u8 *addr)
{
	NRF_TxPacket_AP(addr,32);
	//NRF_TxPacket(addr,32);
}

void Nrf_Check_Event(void)	//将2401接收到的数据返回串口
{
	u8 sta = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<RX_DR))//接收中断
	{
		u8 rx_len = NRF_Read_Reg(R_RX_PL_WID);	//2401寄存器，值为0x60
		if(rx_len<33)
		{
			NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);// read receive payload from RX_FIFO buffer
			Uart1_Send_Buf(NRF24L01_RXDATA,rx_len);
		}
		else 
		{
			NRF_Write_Reg(FLUSH_RX,0xff);//清空缓冲区
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<TX_DS))//发送中断
	{
		//PC_Debug_Show(1,2);
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<MAX_RT))//应该是缓存溢出中断
	{
		static u16 max_cnt = 0;
		max_cnt++;
		PC_Debug_Show(7,max_cnt);
		
		if(sta & 0x01)	//TX FIFO FULL
		{
			NRF_Write_Reg(FLUSH_TX,0xff);
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);//??nrf??????
}
void NRF_Send_RC(void)
{
	NRF24L01_TXDATA_RC[0] = 0x8A;	//源程序中为8A	遥控指令
	NRF24L01_TXDATA_RC[1] = 0x8A;	//源程序中为8A	遥控指令
	NRF24L01_TXDATA_RC[2] = 0x1C;
	NRF24L01_TXDATA_RC[3] = BYTE1(Rc_Get.THROTTLE);
	NRF24L01_TXDATA_RC[4] = BYTE0(Rc_Get.THROTTLE);

	NRF24L01_TXDATA_RC[5] = BYTE1(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[6] = BYTE0(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[7] = BYTE1(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[8] = BYTE0(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[9] = BYTE1(Rc_Get.PITCH);
	NRF24L01_TXDATA_RC[10] = BYTE0(Rc_Get.PITCH);
	
	NRF24L01_TXDATA_RC[11] = BYTE1(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[12] = BYTE0(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[13] = BYTE1(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[14] = BYTE0(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[15] = BYTE1(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[16] = BYTE0(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[17] = BYTE1(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[18] = BYTE0(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[19] = BYTE1(Rc_Get.AUX5);
	NRF24L01_TXDATA_RC[20] = BYTE0(Rc_Get.AUX5);
	
	u8 sum = 0;
	for(u8 i=0;i<31;i++)
		sum += NRF24L01_TXDATA_RC[i];
	NRF24L01_TXDATA_RC[31] = sum;
	NRF_SEND_RC_FLAG = 1;
}



