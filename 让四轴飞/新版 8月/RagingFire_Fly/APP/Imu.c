 #include "imu.h"
#include "math.h"
#include "control.h"

#define RtA 		57.324841f		//弧度到角度		
#define AtR    		0.0174533f		//度到角度		
#define Acc_G 		0.0011963f		//加速度变成G		
#define Gyro_G 		0.0610351f		//角速度变成度   此参数对应陀螺2000度每秒		
#define Gyro_Gr		0.0010653f		//角速度变成弧度 此参数对应陀螺2000度每秒	
#define FILTER_NUM 	20

float 	AngleOffset_Rol=0,AngleOffset_Pit=0;

void Prepare_Data(T_int16_xyz *acc_in,T_int16_xyz *acc_out)
{
	static uint8_t 	filter_cnt=0;
	static int16_t	ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;

	ACC_X_BUF[filter_cnt] = acc_in->X;
	ACC_Y_BUF[filter_cnt] = acc_in->Y;
	ACC_Z_BUF[filter_cnt] = acc_in->Z;
	for(i=0;i<FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	acc_out->X = temp1 / FILTER_NUM;
	acc_out->Y = temp2 / FILTER_NUM;
	acc_out->Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==FILTER_NUM)	filter_cnt=0;
}
////////////////////////////////////////////////////////////////////////////////
#define Kp 1.6f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f                      // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period 采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;  // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0; // scaled integral error
void IMUupdate(T_int16_xyz *gyr, T_int16_xyz *acc, T_float_angle *angle)
{
	float ax = acc->X,ay = acc->Y,az = acc->Z;
	float gx = gyr->X,gy = gyr->Y,gz = gyr->Z;
	float norm;
	
	float vx, vy, vz;
	float ex, ey, ez;

// 先把这些用得到的值算好
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
	float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	if(ax*ay*az==0)
	return;

	gx *= Gyro_Gr;
	gy *= Gyro_Gr;
	gz *= Gyro_Gr;

	norm = sqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
	ax = ax /norm;
	ay = ay / norm;
	az = az / norm;
	
//	if(norm>16500)
//	{Rc_C.ARMED=0;}
	
	// estimated direction of gravity and flux (v and w)      估计重力方向和流量/变迁
	// 这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
	// 根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
	// 所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。      
	vx = 2*(q1q3 - q0q2);												
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) ;     	//向量外积在相减得到差分就是误差                   					
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;	//对误差进行积分
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;	//将误差PI后补偿到陀螺仪，即补偿积分漂移
	gy = gy + Kp*ey + eyInt;	
	gz = gz + Kp*ez + ezInt;	//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

	// integrate quaternion rate and normalise						  
	//四元素的微分方程，一阶毕卡求解法，更新四元数
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	angle->yaw += -gyr->Z*Gyro_G*0.002f;
	angle->rol = -asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3 - AngleOffset_Pit; // pitch
	angle->pit = -atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3 - AngleOffset_Rol; // roll
}

