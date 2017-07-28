#include "FILTER.h"
#include "stdint.h"


#define  N  10
float value_buff[N];
char i=0;

float filter_huadong(float data)
{
	char count;
	float sum=0;
	value_buff[i++]=data;
	if(i==N)
		i=0;
	for(count=0;count<N;count++)
		sum+=value_buff[count];
	return (sum/N);
}

#define a 0.5f
float value;
float filter_guanxing(float data)
{
	float new_value;
	new_value = data;
	return (1-a)*value + a*new_value; 
}

#define Pi 	3.1415926f
//卡尔曼噪声参数
#define dt           0.04f
#define R_angle      0.5f
#define Q_angle      0.001f
#define Q_gyro       0.003f//越大越滞后

//卡尔曼参数
char  C_0 = 1;
float Q_bias_x, Q_bias_y, Q_bias_z;
float Angle_err_x, Angle_err_y, Angle_err_z;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = { 0,0,0,0 };
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

float Kalman_Filter(float Accel, float Gyro) //卡尔曼函数		
{
	static float out;
	out += (Gyro - Q_bias_y) * dt; //先验估计

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_y = Accel - out;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	out += K_0 * Angle_err_y;	 //后验估计
	Q_bias_y += K_1 * Angle_err_y;	 //后验估计
	Gyro = Gyro - Q_bias_y;	 //输出值(后验估计)的微分=角速度
	return out;
}
















