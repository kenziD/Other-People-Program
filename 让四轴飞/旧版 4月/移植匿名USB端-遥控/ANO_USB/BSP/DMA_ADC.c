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
#include "stdio.h"
#include "LCD5110.h"


#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#define N 1	
#define M 8 	
extern u16 dianya0,dianya1;

unsigned char i;

u16 AD_Value[N][M]; 

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

void PlayDY(void)
{
			switch (dianya0)
			{
				case 42:	
				case 41:	PutIcon((char*)(icon + 6),0,0);
					break;
				case 40:	PutIcon((char*)(icon + 5),0,0);
					break;
				case 39:	PutIcon((char*)(icon + 4),0,0);
					break;
				case 38:	PutIcon((char*)(icon + 3),0,0);
					break;
				case 37:	PutIcon((char*)(icon + 2),0,0);
					break;
				case 36:	PutIcon((char*)(icon + 1),0,0);
					break;
				default:	PutIcon((char*)(icon + 0),0,0);
					break;
			}
			
			switch (dianya1)
			{
				case 42:	
				case 41:	PutIcon((char*)(icon + 6),1,0);
					break;
				case 40:	PutIcon((char*)(icon + 5),1,0);
					break;
				case 39:	PutIcon((char*)(icon + 4),1,0);
					break;
				case 38:	PutIcon((char*)(icon + 3),1,0);
					break;
				case 37:	PutIcon((char*)(icon + 2),1,0);
					break;
				case 36:	PutIcon((char*)(icon + 1),1,0);
					break;
				default:	PutIcon((char*)(icon + 0),1,0);
					break;
			}
}

