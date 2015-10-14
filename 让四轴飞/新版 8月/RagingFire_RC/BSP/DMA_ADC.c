
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
  	/* 使能各个用到的外设时钟 */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE); //使能ADC1通道时钟，各个管脚时钟

		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC最大时间不能超过14M
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输	
}

void GPIO_Configuration(void)
{
	/* 定义 GPIO 初始化结构体 GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;
	
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
}
void ADC_Configuration(void)
{
	/* 定义 ADC 初始化结构体 ADC_InitStructure */
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值
	/*	
	*	独立工作模式;
	*	多通道扫描模式;
	*	连续模数转换模式;
	*	转换触发方式：转换由软件触发启动;
	*	ADC 数据右对齐 ;
	*	进行规则转换的 ADC 通道的数目为1; 
	*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//////////////////////////////////////
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = M;
	ADC_Init(ADC1, &ADC_InitStructure);
	
//	/* 设置 ADC1 使用8转换通道，在规则组里的转换顺序--->1，采样时间为 7 周期 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_7Cycles5 );

	// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* 复位 ADC1 的校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/* 等待 ADC1 校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));	 //复位之前的statue为SET->0,   复位之后的状态为RESRT-->1
	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待 ADC1 校准完成 */
	while(ADC_GetCalibrationStatus(ADC1));
}


void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = N*M; //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道
}

void AD_DMA_Int(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	DMA_Configuration();

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//启动 ADC1 转换 
	DMA_Cmd(DMA1_Channel1, ENABLE);         //启动 DMA 	通道
}
void PlayDY(void)
{
	switch (dianya0)	//四轴
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
	
	switch (dianya1)	//电池
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
	
	if(dianya1 < 37 && dianya1 > 20)		//遥控电压
	{
		LED1_ON;
		LED2_ON;	//绿
	}
	if(dianya0 < 36 && dianya0 > 20)	//四轴电压
	{
		LED1_ON;
		LED3_ON;	//红
	}
}

void PlayPID(void)
{
	shuzi(Rc_Get.AUX1,3,0);
	shuzi(Rc_Get.AUX2,3,5);
	shuzi(Rc_Get.AUX3,3,10);
}

