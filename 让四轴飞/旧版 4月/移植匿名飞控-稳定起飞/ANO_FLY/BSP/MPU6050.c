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
#include "MPU6050.h"
//#include "ANO_Tech_STM32F10x_I2C.h"
#include "tim.h"
#include "BSP.h"  //仅用于外部声明 EE_SAVE_ACC_OFFSET(); EE_SAVE_GYRO_OFFSET();

u8						mpu6050_buffer[14];					//iic读取后存放数据
S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂
u8						GYRO_OFFSET_OK = 0;
u8						ACC_OFFSET_OK = 0;
S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值

void Delay_ms_mpu(u16 nms)
{	
	u8 delay_cnt = TIM3_IRQCNT;
	while((delay_cnt+(nms*2)) > TIM3_IRQCNT);
} 
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器
*******************************************************************************/
void MPU6050_Dataanl(void)
{
	MPU6050_ACC_LAST.X=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;	//减去零偏
	MPU6050_ACC_LAST.Y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;
	//跳过温度ADC
	MPU6050_GYRO_LAST.X=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]) - GYRO_OFFSET.X;
	MPU6050_GYRO_LAST.Y=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) - GYRO_OFFSET.Y;
	MPU6050_GYRO_LAST.Z=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) - GYRO_OFFSET.Z;
	
	if(!GYRO_OFFSET_OK)	//进行零偏计算
	{
		static int32_t	tempgx=0,tempgy=0,tempgz=0;
		static uint8_t cnt_g=0;
// 		LED1_ON;
		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;	//防止以后清零静态变量
			return;
		}
		tempgx+= MPU6050_GYRO_LAST.X;
		tempgy+= MPU6050_GYRO_LAST.Y;
		tempgz+= MPU6050_GYRO_LAST.Z;
		if(cnt_g==200)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			EE_SAVE_GYRO_OFFSET();//保存数据
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;
// 		LED1_ON;
		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= MPU6050_ACC_LAST.X;
		tempay+= MPU6050_ACC_LAST.Y;
		//tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			EE_SAVE_ACC_OFFSET();//保存数据
			return;
		}
		cnt_a++;		
	}
}







/**************************实现函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_Init(void)
{
////	MPU6050_setSleepEnabled(0); //进入工作状态
////	Delay_ms_mpu(200);
////	MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟  0x6b   0x01
////	Delay_ms_mpu(50);
////	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒
////	Delay_ms_mpu(50);
////	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4);	//加速度度最大量程 +-4G
////	Delay_ms_mpu(50);
////	MPU6050_setDLPF(MPU6050_DLPF_BW_42);
////	Delay_ms_mpu(50);
////	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
////	Delay_ms_mpu(50);
////	MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
////	Delay_ms_mpu(50);




}
///**************************实现函数********************************************
////	改写的匿名程序	ANO_Tech_I2C1_Read_Int
//*******************************************************************************/
//void ANO_Tech_I2C1_Read_Int(u8 Addr,u8 MPU6050,u8 num,u8 *mpu_buffer)
//{
//}
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
*******************************************************************************/
void MPU6050_Read(void)
{
//		ANO_Tech_I2C1_Read_Int(devAddr,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer);
	
	
	
	
	
	
	
	
	
	
}

///**************************实现函数********************************************
////	改写的匿名程序	ANO_Tech_I2C1_Read_Buf
//*******************************************************************************/

///**************************实现函数********************************************
//*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
//*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
//输入	dev  目标设备地址
//reg	   寄存器地址
//bitNum  要修改目标字节的bitNum位
//data  为0 时，目标位将被清0 否则将被置位
//返回   成功 为1 
//失败为0
//*******************************************************************************/ 
//void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
//	u8 b;
//	ANO_Tech_I2C1_Read_Buf(dev, reg, 1, &b);
//	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//	ANO_Tech_I2C1_Write_1Byte(dev, reg, b);
//}
///**************************实现函数********************************************
//*函数原型:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
//输入	dev  目标设备地址
//reg	   寄存器地址
//bitStart  目标字节的起始位
//length   位长度
//data    存放改变目标字节位的值
//返回   成功 为1 
//失败为0
//*******************************************************************************/ 
//void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//{
//	
//	u8 b,mask;
//	ANO_Tech_I2C1_Read_Buf(dev, reg, 1, &b);
//	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
//	data <<= (8 - length);
//	data >>= (7 - bitStart);
//	b &= mask;
//	b |= data;
//	ANO_Tech_I2C1_Write_1Byte(dev, reg, b);
//}
///**************************实现函数********************************************
//*函数原型:		void MPU6050_setClockSource(uint8_t source)
//*功　　能:	    设置  MPU6050 的时钟源
//* CLK_SEL | Clock Source
//* --------+--------------------------------------
//* 0       | Internal oscillator
//* 1       | PLL with X Gyro reference
//* 2       | PLL with Y Gyro reference
//* 3       | PLL with Z Gyro reference
//* 4       | PLL with external 32.768kHz reference
//* 5       | PLL with external 19.2MHz reference
//* 6       | Reserved
//* 7       | Stops the clock and keeps the timing generator in reset
//*******************************************************************************/
//void MPU6050_setClockSource(uint8_t source){
//	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
//	
//}
///** Set full-scale gyroscope range.
//* @param range New full-scale gyroscope range value
//* @see getFullScaleRange()
//* @see MPU6050_GYRO_FS_250
//* @see MPU6050_RA_GYRO_CONFIG
//* @see MPU6050_GCONFIG_FS_SEL_BIT
//* @see MPU6050_GCONFIG_FS_SEL_LENGTH
//*/
//void MPU6050_setFullScaleGyroRange(uint8_t range) {
//	IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
//*功　　能:	    设置  MPU6050 加速度计的最大量程
//*******************************************************************************/
//void MPU6050_setFullScaleAccelRange(uint8_t range) {
//	IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
//}
///**************************实现函数********************************************
//*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
//*功　　能:	    设置  MPU6050 是否进入睡眠模式
//enabled =1   睡觉
//enabled =0   工作
//*******************************************************************************/
//void MPU6050_setSleepEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
//*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
//*******************************************************************************/
//void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
//*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
//*******************************************************************************/
//void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
//}

//void MPU6050_setDLPF(uint8_t mode)
//{
//	IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
//}

