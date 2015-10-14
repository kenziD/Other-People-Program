
#include "rc.h"
#include "nrf24l01.h"
#include "uart/uart1.h"

/////////////////////////////////////////////////////////
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define RX_DR		6		//�жϱ�־
#define TX_DS		5
#define MAX_RT		4

RC_GETDATA Rc_Get;//���յ���RC����,1000~2000
u8 NRF_SEND_RC_FLAG = 0;
//////////////////////////////////////////////////////////////////////
static u16 Nrf_Erro;
extern u16 dianya0;	//�����ѹ
//////////////////////////////////////////////////////////////////////

void Data_Receive_Anl(u8 *data_buf,u8 num)
{
	vs16 rc_value_temp;
	u8 sum = 0;
	
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAA))		return;		
/////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0X03)
	{
		dianya0 = (vs16)(*(data_buf+20)<<8)|*(data_buf+21);
	}
}

void NRF_SendData_AP(u8 *addr)
{
	NRF_TxPacket_AP(addr,32);
}

void Nrf_Check_Event(void)	//��2401���յ������ݷ��ش���
{
	u8 sta = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<RX_DR))//�����ж�
	{
		u8 rx_len = NRF_Read_Reg(R_RX_PL_WID);//2401�Ĵ�����ֵΪ0x60
		if(rx_len<33)
		{
			NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);// read receive payload from RX_FIFO buffer
			Data_Receive_Anl(NRF24L01_RXDATA,rx_len);		 //�������յ���2401����
			Uart1_Send_Buf(NRF24L01_RXDATA,rx_len);
		}
		else 
		{
			NRF_Write_Reg(FLUSH_RX,0xff);//��ջ�����
		}
		Nrf_Erro = 0;
	}
	else
	{
		Nrf_Erro ++;
		if(Nrf_Erro>1000)	
		{
			Nrf_Erro = 0;
			dianya0 = 0;
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<TX_DS))//�����ж�
	{
		//PC_Debug_Show(1,2);
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<MAX_RT))//Ӧ���ǻ�������ж�
	{
		static u16 max_cnt = 0;
		max_cnt++;
		
		if(sta & 0x01)	//TX FIFO FULL
		{
			NRF_Write_Reg(FLUSH_TX,0xff);
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);
}
void NRF_Send_RC(void)
{
	NRF24L01_TXDATA_RC[0] = 0xAA;	
	NRF24L01_TXDATA_RC[1] = 0xAF;
	NRF24L01_TXDATA_RC[2] = 0x03;
	
	NRF24L01_TXDATA_RC[3] = 31;
	
	NRF24L01_TXDATA_RC[4] = BYTE1(Rc_Get.THROTTLE);
	NRF24L01_TXDATA_RC[5] = BYTE0(Rc_Get.THROTTLE);

	NRF24L01_TXDATA_RC[6] = BYTE1(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[7] = BYTE0(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[8] = BYTE1(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[9] = BYTE0(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[10] = BYTE1(Rc_Get.PITCH);
	NRF24L01_TXDATA_RC[11] = BYTE0(Rc_Get.PITCH);
	
	NRF24L01_TXDATA_RC[12] = BYTE1(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[13] = BYTE0(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[14] = BYTE1(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[15] = BYTE0(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[16] = BYTE1(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[17] = BYTE0(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[18] = BYTE1(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[19] = BYTE0(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[20] = BYTE1(Rc_Get.AUX5);
	NRF24L01_TXDATA_RC[21] = BYTE0(Rc_Get.AUX5);
	NRF24L01_TXDATA_RC[22] = BYTE1(Rc_Get.AUX6);
	NRF24L01_TXDATA_RC[23] = BYTE0(Rc_Get.AUX6);
	
	u8 sum = 0;
	for(u8 i=0;i<30;i++)
		sum += NRF24L01_TXDATA_RC[i];
	NRF24L01_TXDATA_RC[31] = sum;
	
	NRF_SEND_RC_FLAG = 1;
}



