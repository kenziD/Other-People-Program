
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#include "led.h"
#include "5110/LCD5110.h"
#include "rc/rc.h"

//////////////////////////////////////////////////////////////////////
#define N 1	
#define M 8 	
u16 AD_Value[N][M]; 
//////////////////////////////////////////////////////////////////////
extern u16 dianya0,dianya1;
void shuzi(u16 value,u8 column,u8 row);
//////////////////////////////////////////////////////////////////////

void RCC_Configuration(void)
{  	
  	/* ʹ�ܸ����õ�������ʱ�� */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE); //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��

		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC���ʱ�䲻�ܳ���14M
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA����	
}

void GPIO_Configuration(void)
{
	/* ���� GPIO ��ʼ���ṹ�� GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;
	
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
}
void ADC_Configuration(void)
{
	/* ���� ADC ��ʼ���ṹ�� ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1); //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	/*	
	*	��������ģʽ;
	*	��ͨ��ɨ��ģʽ;
	*	����ģ��ת��ģʽ;
	*	ת��������ʽ��ת���������������;
	*	ADC �����Ҷ��� ;
	*	���й���ת���� ADC ͨ������ĿΪ1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//////////////////////////////////////
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = M;
	ADC_Init(ADC1, &ADC_InitStructure);
	
//	/* ���� ADC1 ʹ��8ת��ͨ�����ڹ��������ת��˳��--->1������ʱ��Ϊ 7 ���� */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_7Cycles5 );

	// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
	ADC_DMACmd(ADC1, ENABLE);

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* ��λ ADC1 ��У׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* �ȴ� ADC1 У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));	 //��λ֮ǰ��statueΪSET->0,   ��λ֮���״̬ΪRESRT-->1
	/* ��ʼ ADC1 У׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ� ADC1 У׼��� */
	while(ADC_GetCalibrationStatus(ADC1));
}


void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = N*M; //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
}

void AD_DMA_Int(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	DMA_Configuration();

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//���� ADC1 ת�� 
	DMA_Cmd(DMA1_Channel1, ENABLE);         //���� DMA 	ͨ��
}
void PlayDY(void)
{
	switch (dianya0)	//����
	{
		case 42:	PutIcon((char*)(icon + 7),0,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(2+ 0x10);
		case 41:	PutIcon((char*)(icon + 6),0,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(1+ 0x10);
			break;
		case 40:	PutIcon((char*)(icon + 5),0,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(0+ 0x10);
			break;
		case 39:	PutIcon((char*)(icon + 4),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(9+ 0x10);
			break;
		case 38:	PutIcon((char*)(icon + 3),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(8+ 0x10);
			break;
		case 37:	PutIcon((char*)(icon + 2),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(7+ 0x10);
			break;
		case 36:	PutIcon((char*)(icon + 1),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(6+ 0x10);
			break;
		case 35:	PutIcon((char*)(icon + 0),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(5+ 0x10);
			break;
		case 34:	PutIcon((char*)(icon + 0),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(4+ 0x10);
			break;
		case 33:	PutIcon((char*)(icon + 0),0,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(3+ 0x10);
			break;
		default:	PutIcon((char*)(icon + 0),0,0);
					WriteChar(0+ 0x10);WriteChar(0x0e);WriteChar(0+ 0x10);
			break;
	}
	
	switch (dianya1)	//���
	{
		case 42:	PutIcon((char*)(icon + 7),1,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(2+ 0x10);
			break;
		case 41:	PutIcon((char*)(icon + 6),1,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(1+ 0x10);
			break;
		case 40:	PutIcon((char*)(icon + 5),1,0);
					WriteChar(4+ 0x10);WriteChar(0x0e);WriteChar(0+ 0x10);
			break;
		case 39:	PutIcon((char*)(icon + 4),1,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(9+ 0x10);
			break;
		case 38:	PutIcon((char*)(icon + 3),1,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(8+ 0x10);
			break;
		case 37:	PutIcon((char*)(icon + 2),1,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(7+ 0x10);
			break;
		case 36:	PutIcon((char*)(icon + 1),1,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(6+ 0x10);
			break;
		case 35:	PutIcon((char*)(icon + 0),1,0);
					WriteChar(3+ 0x10);WriteChar(0x0e);WriteChar(5+ 0x10);
			break;
		default:	PutIcon((char*)(icon + 0),1,0);
					WriteChar(0+ 0x10);WriteChar(0x0e);WriteChar(0+ 0x10);
			break;
	}
	
	LEDALL_OFF
	
	if(dianya1 < 37 && dianya1 > 20)		//ң�ص�ѹ
	{
		LED1_ON;
		LED2_ON;	//��
	}
	if(dianya0 < 36 && dianya0 > 20)	//�����ѹ
	{
		LED1_ON;
		LED3_ON;	//��
	}
}

void PlayPID(void)
{
	shuzi(Rc_Get.AUX1,3,0);
	shuzi(Rc_Get.AUX2,3,5);
	shuzi(Rc_Get.AUX3,3,10);
}

