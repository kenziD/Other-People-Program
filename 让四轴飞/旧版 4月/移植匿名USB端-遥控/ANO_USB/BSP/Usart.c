///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ĩ�μ��˱���������ɳ����������˼�Ļ��ͨ���������ʶ�������ø��ܶ������������������Ŷӣ�һ�����о����ᣡ
//�������������˼���û�У���˳������ֲ�͵��Զ����Լ�������ʱ���������ģ�Ҳ�������ʡ�
//������EEPW�����̳�Խ��Խ��������ڷ��׳��Լ��Ĵ��룬���Ǹ��õ�����
//��Ϊֻ�в��Ͻ���������ȡ�ø���Ľ�����������Ƥ�������������~ 
//    
//�����������Ҿ������Լ���ֲ���ԵĴ��뿪Դ�����������ѧϰ��ϣ�������һ�����������������������ǡ�
//��Ϊ���ǵ��׼�����һ���ģ�Ӳ������һ�µ��ˣ��Ǿ�ֻ��Ҫ�о�����ϵ��㷨�����ˡ����������᷽��Ķ࣡
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����DIY����飺http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//������̳��http://forum.eepw.com.cn/forum/368/1 
//
//�ҵ�����DIY��������http://forum.eepw.com.cn/thread/248747/1
//
//�Ա����̣�http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:������ɣ�2014,4,30
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef union {unsigned char byte[4];float num;}t_floattobyte;
t_floattobyte floattobyte;

void Uart1_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	//����PA9��ΪUSART1��Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//����PA10��ΪUSART1��Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	//����USART1
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART1ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	//ʹ��USART1�����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(USART1, ENABLE); 
}
u8 TxBuffer[0xff];
u8 TxCounter=0;
u8 count=0; 
u8 Rx_Buf[2][32];	//����32�ֽڵĴ��ڽ��ջ���
u8 Rx_Act=0;		//����ʹ�õ�buf��
u8 Rx_Adr=0;		//���ڽ��յڼ��ֽ�
u8 Rx_Ok0 = 0;
u8 Rx_Ok1 = 0;

void Uart1_IRQ(void)
{
	if(USART1->SR & USART_SR_ORE)//ORE�ж�
	{
		u8 com_data = USART1->DR;
		//USART_ClearFlag(USART1,USART_IT_ORE);
	}
	//�����ж�
	if((USART1->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))
	//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
	{
		USART1->DR = TxBuffer[TxCounter++]; //дDR����жϱ�־          
		if(TxCounter == count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�
			//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
		}
	}
	//�����ж� (���ռĴ����ǿ�) 
	if(USART1->SR & (1<<5))
	//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
	{
		u8 com_data = USART1->DR;
		if(Rx_Adr==0)		//Ѱ��֡ͷ0X8A
		{
			if(com_data==0x8A)	//�������������0X8A,��д�뻺��
			{
				Rx_Buf[Rx_Act][0] = com_data;
				Rx_Adr = 1;
			}
		}
		else		//���ڽ�������
		{
			Rx_Buf[Rx_Act][Rx_Adr] = com_data;
			Rx_Adr ++;
		}
		if(Rx_Adr==32)		//���ݽ������
		{
			Rx_Adr = 0;
			if(Rx_Act)	
			{ 
				Rx_Act = 0; 			//�л�����
				Rx_Ok1 = 1;
			}
			else 				
			{
				Rx_Act = 1;
				Rx_Ok0 = 1;
			}
		}
	}
}
/**************************ʵ�ֺ���********************************************
*******************************************************************************/
uint8_t Uart1_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;
  if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
	return DataToSend;
}
uint8_t Uart1_Put_Int16(uint16_t DataToSend)
{
	uint8_t sum = 0;
	TxBuffer[count++] = BYTE1(DataToSend);
	TxBuffer[count++] = BYTE0(DataToSend);
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	sum += BYTE1(DataToSend);
	sum += BYTE0(DataToSend);
	return sum;
}
uint8_t Uart1_Put_Float(float DataToSend)
{
	uint8_t sum = 0;
	floattobyte.num=DataToSend;
	TxBuffer[count++] = floattobyte.byte[3];  
	TxBuffer[count++] = floattobyte.byte[2];  
	TxBuffer[count++] = floattobyte.byte[1];  
	TxBuffer[count++] = floattobyte.byte[0];  
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	sum += BYTE3(DataToSend);
	sum += BYTE2(DataToSend);
	sum += BYTE1(DataToSend);
	sum += BYTE0(DataToSend);
	return sum;	
}
void Uart1_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str)
	{
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')Uart1_Put_Char(0x0d);
		else if(*Str=='\n')Uart1_Put_Char(0x0a);
			else Uart1_Put_Char(*Str);
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}

