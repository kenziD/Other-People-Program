#ifndef _MMA7660_H_
#define _MMA7660_H_
#include "stm32f10x.h"



typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;

extern S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		
extern S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			
extern u8							GYRO_OFFSET_OK;
extern u8							ACC_OFFSET_OK;

void MPU6050_Read(void);
void MPU6050_Dataanl(void);

/*
 **********************************************************
 *
 * 定义MPU6050内部地址
 *
 **********************************************************
 */
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x04，五分频
#define	CONFIG2			  0x1A	//低通滤波频率，典型值：0x02(截止频率100Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x10(不自检，8G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器的地址(默认数值0x68，只读!!!)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取-------0xD0 = 11010000; 0x68 = 1101000;  

/*
 **********************************************************
 *
 * 相关宏定义
 *
 **********************************************************
 */
//#define ERROR			0
//#define OK				1

#define	SDA_D	  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)	//PC10读数据
#define	SDA_H   GPIO_SetBits(GPIOB, GPIO_Pin_11)	 // PC10 高电平
#define	SDA_L   GPIO_ResetBits(GPIOB, GPIO_Pin_11)	 // PC10 低电平
						
#define	SCL_H   GPIO_SetBits(GPIOB, GPIO_Pin_10)	 // PC8 高电平
#define	SCL_L   GPIO_ResetBits(GPIOB, GPIO_Pin_10)	 // PC8 低电平

/*
 **********************************************************
 *
 * 全局函数声明
 *
 **********************************************************
 */
void SDA_IOOUT(void);								//SDA端口配置为输出
void SDA_IOIN(void);								//SDA端口配置为输入
void I2C_Start(void);									//开始信号
void I2C_Stop(void);									//结束信号
unsigned char I2C_SlaveAck(void);						//应答信号
void I2C_WriteByte(unsigned char a);					//写一字节
unsigned char I2C_ReadByte(void);						//读一字节
void I2C_GPIO_Config(void);

void Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data); //写寄存器
unsigned char Single_ReadI2C(unsigned char Regs_Addr);	//读寄存器

void InitMPU6050(void);
int GetData(u8 REG_Address);
void MPU6050_Dataanl(void);


#endif

