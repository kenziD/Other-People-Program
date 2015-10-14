#ifndef _BSP_BSP_H_
#define _BSP_BSP_H_
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "nrf24l01.h"
#include "led.h"
//#include "ANO_Tech_STM32F10x_I2C.h"
#include "app/I2C_6050/I2C_MPU6050.h"	
#include "moto.h"
#include "eeprom.h"
#include "adc.h"

extern u8 SYS_INIT_OK;

void MCO_INIT(void);
void Nvic_Init(void);

void EE_INIT(void);
void EE_SAVE_ACC_OFFSET(void);
void EE_READ_ACC_OFFSET(void);
void EE_SAVE_GYRO_OFFSET(void);
void EE_READ_GYRO_OFFSET(void);
void EE_SAVE_PID(void);
void EE_READ_PID(void);

#endif
