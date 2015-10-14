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
//论坛：http://forum.eepw.com.cn/forum/368/1 
//
//淘宝店铺：http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "I2C_MPU6050.h"
/***************************************************************/
#include "BSP/tim.h"	//定用于精确定时
#include "BSP/BSP.h"  //仅用于外部声明 EE_SAVE_ACC_OFFSET(); EE_SAVE_GYRO_OFFSET();

u8						mpu6050_buffer[14];					//iic读取后存放数据
S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂
u8						GYRO_OFFSET_OK = 0;	//不自动校正
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




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
//				以上为匿名源程序，有所屏蔽
//
//			  以下为移植的 I2C+6050 程序
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void SDA_IOOUT(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;		// 开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	    
}

void SDA_IOIN(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;  // 浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);				     			
}

static void I2C_delay(void)
{
    volatile int i = 1;
    while (i)
        i--;
}

/*
 *=========================================================
 * 函数功能：IIC总线初始化信号
 * 参数：无
 * 函数返回值：无 
 * SDA    ---
 *           |________
 * SCL ----------
 *               |____  
 *=========================================================   
 */
void I2C_Start(void)
{
	SCL_L;					
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_L;
	I2C_delay();  
} 

/*
 *=========================================================
 * 函数功能：IIC总线停止信号
 * 参数：无
 * 函数返回值：无 
 * SDA        ---------
 *      ______|
 * SCL   ---------
 *                |___
 *=========================================================   
 */
void I2C_Stop(void)
{
	SCL_L;					
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}
/*
 *=========================================================
 * 函数功能：接收由从机向主机发起的ACK信号。
 * 参数：	 ACKBIT
 *          
 * 函数返回值：MMA_ERROR/MMA_OK   
 *=========================================================   
 */
unsigned char I2C_SlaveAck(void)
{
	SCL_L;				
	I2C_delay();
	SDA_H;
	
	SDA_IOIN();					//SDA设为输入
	I2C_delay();
	SCL_H;
	
	I2C_delay();
	
	if(SDA_D)
	{
		SDA_IOOUT();			//SDA设为输出
		SCL_L;
		return ERROR;			//返回错误	
	}
	SCL_L;
	SDA_IOOUT();					//SDA设为输出
	I2C_delay();
	return SUCCESS;					//返回正常	  
}

/*
 *=========================================================
 * 函数功能：IIC写一个字节数据
 * 参数： 	 a：需要写入的一个字节数据
 * 函数返回值：无
 *=========================================================   
 */
void I2C_WriteByte(unsigned char a) 
{
	unsigned char i; 
	for(i=0; i<8; i++)
	{	  	
		SCL_L;
		if((a&0x80)!=0) SDA_H;
		else SDA_L;
		a <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
	}   
	SCL_L;  

	if(I2C_SlaveAck()==ERROR)		//等待从机的ACK信号。
	{
		return ;
	}
}

/*
 *=========================================================
 * 函数功能：IIC读一个字节数据
 * 参数：无
 * 函数返回值：返回读出的一个字节数据
 *=========================================================   
 */
unsigned char I2C_ReadByte(void)
{
	unsigned char a =0;
	unsigned char i;
	SDA_IOIN();					//SDA设为输入	
	for(i=0; i<8; i++)
	{
 		a <<= 1;  
		SCL_H;
		I2C_delay();
		if(SDA_D==1) a |= 0x01;
    	I2C_delay();
		SCL_L;
	  	I2C_delay();   
	}
	return a;
}

/*
 *=========================================================
 * 函数功能：写MPU6050寄存器
 * 参数：     
 *       Regs_Addr - 寄存器地址
 *       Regs_Data - 寄存器值
 * 函数返回值：
 *=========================================================   
 */
void Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data) 
{  
   I2C_Start();
   I2C_WriteByte(SlaveAddress); 		//先写Slave地址，并配置成写模式
   I2C_WriteByte(Regs_Addr);    		//写寄存器地址
   I2C_WriteByte(Regs_Data);    		//写寄存器内容
   I2C_Stop();                  		//结束本段IIC进程
}

/*
 *=========================================================
 * 函数功能：读MPU6050单字节
 * 参数
 *       Regs_Addr - 寄存器地址
 * 函数返回值：寄存器值
 *=========================================================   
 */
unsigned char Single_ReadI2C(unsigned char Regs_Addr) 
{
   unsigned char ret;
   
   I2C_Start();
   
   I2C_WriteByte(SlaveAddress); 		//先写Slave地址，并配置成写模式
   I2C_WriteByte(Regs_Addr);    		//写寄存器地址
   
   I2C_Start();
   I2C_WriteByte(SlaveAddress+1);  	//写Slave地址，并配置成读模式
   ret=I2C_ReadByte();							//从传感器中读出数据

   I2C_SlaveAck();
   I2C_Stop();                  		//结束本段IIC进程 
   
   return ret;
} 

/*
 **********************************************************
 *
 * 数据合成
 *
 **********************************************************
 */
int GetData(u8 REG_Address)
{
	u8 H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}
/*
 **********************************************************
 *
 * 初始化MPU6050
 *
 **********************************************************
 */
void InitMPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);//电源管理，典型值：0x00(正常启用)
	delay_ms(2);
	Single_WriteI2C(SMPLRT_DIV, 0x04);//陀螺仪采样率，典型值：0x04，五分频
	delay_ms(2);
	Single_WriteI2C(CONFIG2, 0x02);//低通滤波频率，典型值：0x02(截止频率100Hz)
	delay_ms(2);
	Single_WriteI2C(GYRO_CONFIG, 0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	delay_ms(2);						
	Single_WriteI2C(ACCEL_CONFIG, 0x10);//加速计自检、测量范围及高通滤波频率，典型值：0x10(不自检，8G，5Hz)
	delay_ms(2);
}

void MPU6050_Read(void)
{
	mpu6050_buffer[0] = Single_ReadI2C(ACCEL_XOUT_H);
	mpu6050_buffer[1] = Single_ReadI2C(ACCEL_XOUT_L);
	mpu6050_buffer[2] = Single_ReadI2C(ACCEL_YOUT_H);
	mpu6050_buffer[3] = Single_ReadI2C(ACCEL_YOUT_L);
	mpu6050_buffer[4] = Single_ReadI2C(ACCEL_ZOUT_H);
	mpu6050_buffer[5] = Single_ReadI2C(ACCEL_ZOUT_L);
	
	mpu6050_buffer[8] = Single_ReadI2C(GYRO_XOUT_H);
	mpu6050_buffer[9] = Single_ReadI2C(GYRO_XOUT_L);
	mpu6050_buffer[10] = Single_ReadI2C(GYRO_YOUT_H);
	mpu6050_buffer[11] = Single_ReadI2C(GYRO_YOUT_L);
	mpu6050_buffer[12] = Single_ReadI2C(GYRO_ZOUT_H);
	mpu6050_buffer[13] = Single_ReadI2C(GYRO_ZOUT_L);
	
//	ACC.X = GetData(ACCEL_XOUT_H);
//	ACC.Y = GetData(ACCEL_YOUT_H);
//	ACC.Z = GetData(ACCEL_ZOUT_H);

//	GYRO.X = GetData(GYRO_XOUT_H);
//	GYRO.Y = GetData(GYRO_YOUT_H);
//	GYRO.Z = GetData(GYRO_ZOUT_H);
}












