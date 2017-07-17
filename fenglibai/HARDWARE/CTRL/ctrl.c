#include "ctrl.h"
#include "pid.h"
#include "imu.h"
#include "pwm_out.h"
#include "math.h"
#include "pos_mach.h"

extern float Angle_X; //�������궨��Ƕ�
extern float Angle_Y; 
extern float Angle_Z;

float Angle_except;
float Angle_except_x;
float Angle_except_y;

short inc_pwm_x;
short inc_pwm_y;

short x_PWM_High=5000; //X����PWM�������
short x_PWM_Low=5000;  //X����PWM�������

short y_PWM_High=5000; //Y����PWM�������
short y_PWM_Low=5000;  //Y����PWM�������

extern short Kp_err;
extern short Ki_err;
extern short Kd_err;

u16 pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7;

//���������Ϊ�����Ƕ�
float change(float Distant_except)
{
	Angle_except = atan2(Distant_except,0.89)/3.14*180;
	return Angle_except;
}
//�����Ӧx,y����
void CTRL_x(float Distant_x_except)
{
	Angle_except_x = change(Distant_x_except);
	inc_pwm_x = PID_CAL_x(Angle_X,Angle_except_x);
	x_PWM_High += inc_pwm_x;
	x_PWM_Low  -= inc_pwm_x;
	
	//����
	if(x_PWM_High<=5500 && x_PWM_High>5000)
	{
		x_PWM_High =5500;
	}
	if(x_PWM_Low<=5000 && x_PWM_Low>=4500)
	{
		x_PWM_Low =4500;
	}
	
	mpu6050_send_data(x_PWM_High,x_PWM_Low,Angle_except_x,Angle_except_y,Kp_err,Ki_err);
	if(x_PWM_High >10000)
	{
		x_PWM_High = 10000;
	}
	if(x_PWM_Low < 0)
	{
		x_PWM_Low = 0;
	}
	if(Angle_X<0)
	{	
		pwm2 = x_PWM_High;
		pwm6 = x_PWM_High;
		pwm3 = x_PWM_Low;
		pwm7 = x_PWM_Low;
	}
	if(Angle_X>=0)
	{
		pwm2 = x_PWM_Low;
		pwm6 = x_PWM_Low;
		pwm3 = x_PWM_High;
		pwm7 = x_PWM_High;
	}
	SetPwm(pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7);
}
void CTRL_y(float Distant_y_except)
{
	Angle_except_y = change(Distant_y_except);
	inc_pwm_y = PID_CAL_y(Angle_Y,Angle_except_y);
	y_PWM_High += inc_pwm_y;
	y_PWM_Low  -= inc_pwm_y;
	
	//����
	if(y_PWM_High<=5500 && y_PWM_High>=5000)
	{
		y_PWM_High =5500;
	}
	if(y_PWM_Low<=5000 && y_PWM_Low>=4500)
	{
		y_PWM_Low =4500;
	}
	//mpu6050_send_data(x_PWM_High,x_PWM_Low,Angle_except_x,Angle_except_y,Kp_err,Ki_err);
	if(y_PWM_High >10000)
	{
		y_PWM_High = 10000;
	}
	if(y_PWM_Low < 0)
	{
		y_PWM_Low = 0;
	}

	if(Angle_Y<=0)
	{	
		pwm0 = y_PWM_High;
		pwm4 = y_PWM_High;
		pwm1 = y_PWM_Low;
		pwm5 = y_PWM_Low;
	}
	if(Angle_Y>0)
	{
		pwm0 = y_PWM_Low;
		pwm4 = y_PWM_Low;
		pwm1 = y_PWM_High;
		pwm5 = y_PWM_High;
	}
	SetPwm(pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7);
}

float x,y;

//����ǶȺͳ���
void line(float angle_l,float length_l)
{
	x=length_l*sin(angle_l/180*3.1415);
	y=length_l*fabs(cos(angle_l/180*3.1415));
	CTRL_x(x);
	CTRL_y(y);
}
