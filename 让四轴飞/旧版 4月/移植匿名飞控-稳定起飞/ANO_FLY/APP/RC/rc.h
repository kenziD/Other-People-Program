#ifndef _RC_RC_H_
#define _RC_RC_H_
#include "stm32f10x.h"

typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
				int16_t AUX1;
				int16_t AUX2;
				int16_t AUX3;
				int16_t AUX4;
				int16_t AUX5;}RC_GETDATA;

extern RC_GETDATA Rc_Get;//接收到的RC数据,1000~2000

extern float RC_Target_ROL,RC_Target_PIT,RC_Target_YAW;

void Nrf_Check_Event(void);
void NRF_Send_AF(void);
void NRF_Send_AE(void);
void NRF_Send_OFFSET(void);
void NRF_Send_PID(void);
void NRF_Send_ARMED(void);
void NRF_SEND_test(void);

#endif
