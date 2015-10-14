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
#include "control.h"
#include "BSP/BSP.H"
#include "app/rc/rc.h"
#include "app/imu/imu.h"

PID PID_ROL,PID_PIT,PID_YAW;
u8 ARMED = 0;
extern vs16 QH,ZY,XZ;

float Get_MxMi(float num,float max,float min)
{
	if(num>max)
		return max;
	else if(num<min)
		return min;
	else
		return num;
}

void CONTROL(float rol_now, float pit_now, float yaw_now, float rol_tar, float pit_tar, float yaw_tar)	
{						//当前姿态角，，，目标姿态角
	u16 moto1=0,moto2=0,moto3=0,moto4=0;
	vs16 throttle;
	
	float rol = rol_tar + rol_now;
	float pit = pit_tar + pit_now;
	float yaw = yaw_tar + yaw_now;
	
	throttle = Rc_Get.THROTTLE - 1000;	//1000<遥控油门值<2000
	if(throttle<0)	throttle=0;
	
	PID_ROL.IMAX = throttle/10;			//积分限幅，积分值不超过当前油门值的一半
	PID_ROL.IMAX = Get_MxMi(PID_ROL.IMAX,1000,0);	//限制积分结果为，0到1000	
	PID_PIT.IMAX = PID_ROL.IMAX;
	
	PID_ROL.pout = PID_ROL.P * rol;
	PID_PIT.pout = PID_PIT.P * pit;
//////////////////、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、	
	/////////////0.1  0.1   30  30
	if(rol_tar*rol_tar<1 && pit_tar*pit_tar<1 && rol_now*rol_now<100 && pit_now*pit_now<100 && throttle>300)
	{			//防止角度大了，积分超调//目标姿态角水平，姿态角几乎水平，油门值不太低
		PID_ROL.iout += PID_ROL.I * rol;
		PID_PIT.iout += PID_PIT.I * pit;
		PID_ROL.iout = Get_MxMi(PID_ROL.iout,PID_ROL.IMAX,-PID_ROL.IMAX);		//对输出的积分限幅
		PID_PIT.iout = Get_MxMi(PID_PIT.iout,PID_PIT.IMAX,-PID_PIT.IMAX);
	}
	else if(throttle<200)
	{			//油门值较小时，积分项清零
		PID_ROL.iout = 0;
		PID_PIT.iout = 0;
	}
//////////////////、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、	

//	rc_roll_d = rol_tar - getlast_roll;
//	getlast_roll = rol_tar;
//	PID_ROL.dout = PID_ROL.D * (MPU6050_GYRO_LAST.X+rc_roll_d*300);//角速度+控制误差微分
//	
//	rc_pitch_d = pit_tar - getlast_pitch;
//	getlast_pitch = pit_tar;
//	PID_PIT.dout = PID_PIT.D * (MPU6050_GYRO_LAST.Y+rc_pitch_d*300);//角速度+控制误差微分
	
	PID_ROL.dout = PID_ROL.D * MPU6050_GYRO_LAST.X;		
	PID_PIT.dout = PID_PIT.D * MPU6050_GYRO_LAST.Y;
/////////////////////////////////////////////////////////////////////
	PID_YAW.pout = PID_YAW.P * yaw;
	//若 *yaw_now；锁尾模式！！！！
	//若 *yaw；  非锁尾模式！！！！
/////////////////////////////////////////////////////////////////////
	vs16 yaw_d;
	/////////////////////////////////////////////////////////////////////
	if(1480>Rc_Get.YAW || Rc_Get.YAW>1520)	//给遥控加死区（偏航角的死区）
	{
		yaw_d = MPU6050_GYRO_LAST.Z + (Rc_Get.YAW-1500)*10;	//用Z轴角速度及目标偏航角值进行四轴运动的预判
		GYRO_I.Z = 0;
	}
	else
		yaw_d =  MPU6050_GYRO_LAST.Z;
	
	PID_YAW.dout = PID_YAW.D * yaw_d;
/////////////////////////////////////////////////////////////////////	
	PID_ROL.OUT = PID_ROL.pout + PID_ROL.iout + PID_ROL.dout;
	PID_PIT.OUT = PID_PIT.pout + PID_PIT.iout + PID_PIT.dout;
	PID_YAW.OUT = PID_YAW.pout + PID_YAW.iout + PID_YAW.dout;
	
	if(throttle>200)	//油门值大于200才起飞	（遥控油门值大于1200）
	{		
//		moto1 = throttle  - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT;
//		moto2 = throttle  - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT;
//		moto3 = throttle  + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT;
//		moto4 = throttle  + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT;
		moto1 = throttle -  25 + QH + ZY - XZ - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT;
		moto2 = throttle -  25 + QH - ZY + XZ - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT;
		moto3 = throttle -  25 - QH + ZY + XZ + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT;
		moto4 = throttle +  75 - QH - ZY - XZ + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT;
	}
	else
	{
		moto1 = 0;
		moto2 = 0;
		moto3 = 0;
		moto4 = 0;
	}
	if(ARMED)	Moto_PwmRflash(moto1,moto2,moto3,moto4);	//未解锁则空心杯不转	ARMED=1则解锁
	else			Moto_PwmRflash(0,0,0,0);
}

