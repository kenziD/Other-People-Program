#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"
typedef struct PID{float P,pout,I,iout,D,dout,IMAX,OUT;}PID;

extern u8 ARMED;
extern PID PID_ROL,PID_PIT,PID_YAW;

void CONTROL(float rol_now, float pit_now, float yaw_now, float rol_tar, float pit_tar, float yaw_tar);

#endif
