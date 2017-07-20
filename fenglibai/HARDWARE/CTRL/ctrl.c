#include "ctrl.h"
#include "pid.h"
#include "imu.h"
#include "pwm_out.h"
#include "math.h"
#include "pos_mach.h"

extern float Angle_X; //卡尔曼标定后角度
extern float Angle_Y; 
extern float Angle_Z;

float Angle_expect_A; //期望角度幅值
float Angle_expect_x_A;
float Angle_expect_y_A;

float Angle_expect;  //期望实时角度
float Angle_expect_x;
float Angle_expect_y;

float inc_pwm_x;
float inc_pwm_y;

float x_PWM_High=5100; //X方向PWM波高输出
float x_PWM_Low=4900;  //X方向PWM波低输出

float y_PWM_High=5100; //Y方向PWM波高输出
float y_PWM_Low=4900;  //Y方向PWM波低输出

extern float Kp_err_x;
extern float Ki_err_x;
extern float Kd_err_x;

extern float Kp_err_y;
extern float Ki_err_y;
extern float Kd_err_y;

u16 pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7;

u16 nowtime;

float x_A,y_A; 
float x,y;
float bgT = 1600; //摆杆周期ms
float xphase,yphase; //x,y相位
float phase_err=0; //x,y相位差

//期望距离变为期望角度幅值
float change(float Distant_except)
{
	Angle_expect_A = atan2(Distant_except,89)/3.14*180;
	return Angle_expect_A;
}
//输入角度和长度
void way1(float angle_l,float length_l)
{
	nowtime += 10;     //每10ms采样（定时器）
	x_A=length_l*sin(angle_l/180*3.1415);  //x,y长度(赋值)计算
	y_A=length_l*fabs(cos(angle_l/180*3.1415));
	
	if(angle_l<=90)		//大于0小于90直线
	{
		phase_err = 3.1415;
	}
	else
	{
		phase_err = 0;
	}
	
	xphase=2*3.1415*nowtime/bgT; //x,y相位
	yphase=xphase + phase_err;
	
	x = x_A*sin(xphase);
	y = y_A*sin(yphase);
	
	Angle_expect_x = change(x);
	Angle_expect_y = change(y);
	
	inc_pwm_x=PID_CAL_x(Angle_X,Angle_expect_x);
	inc_pwm_y=PID_CAL_y(Angle_Y,Angle_expect_y);

	
	mpu6050_send_data(x_PWM_High,x,y,Angle_expect_x,Angle_expect_y,inc_pwm_x);
	
	if(inc_pwm_x > 0)
	{
		x_PWM_High = 5000 + inc_pwm_x;
		x_PWM_Low  = 5000 - inc_pwm_x;
	}
	else
	{
		x_PWM_Low  = 5000 + inc_pwm_x;
		x_PWM_High = 5000 - inc_pwm_x;
	}
	if(x_PWM_High >= 10000)
	{
		x_PWM_High = 10000;
	}
	if(x_PWM_Low <= 0)
	{
		x_PWM_Low = 0;
	}
	
	if(inc_pwm_y > 0)
	{	
		y_PWM_High = 5000 + inc_pwm_y;
		y_PWM_Low  = 5000 - inc_pwm_y;
	}
	else
	{
		y_PWM_Low  = 5000 + inc_pwm_y;
		y_PWM_High = 5000 - inc_pwm_y;
	}
	if(y_PWM_High >= 10000)
	{
		y_PWM_High = 10000;
	}
	if(y_PWM_Low <= 0)
	{
		y_PWM_Low = 0;
	}

	if(inc_pwm_x > 0)
	{
		pwm2 = x_PWM_Low;
		pwm6 = x_PWM_Low;
		pwm3 = x_PWM_High;
		pwm7 = x_PWM_High;
	}
	else
	{
		pwm2 = x_PWM_High;
		pwm6 = x_PWM_High;
		pwm3 = x_PWM_Low;
		pwm7 = x_PWM_Low;
	}
	
	if(inc_pwm_y > 0)
	{	
		pwm0 = y_PWM_Low;
		pwm4 = y_PWM_Low;
		pwm1 = y_PWM_High;
		pwm5 = y_PWM_High;
	}
	else
	{
		pwm0 = y_PWM_High;
		pwm4 = y_PWM_High;
		pwm1 = y_PWM_Low;
		pwm5 = y_PWM_Low;
	}
	SetPwm(pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7);
	
	if(nowtime >= bgT)
	{
		nowtime=0;
	}

}

//输入角度和长度
void way2(float length_l)
{
	nowtime += 10;     //每10ms采样（定时器）
	
	x_A = length_l;
	y_A = length_l;
	phase_err = 3.1415/2;

	
	xphase=2*3.1415*nowtime/bgT; //x,y相位
	yphase=xphase + phase_err;
	
	x = x_A*sin(xphase);
	y = y_A*sin(yphase);
	
	Angle_expect_x = change(x);
	Angle_expect_y = change(y);
	
	inc_pwm_x=PID_CAL_x(Angle_X,Angle_expect_x);
	inc_pwm_y=PID_CAL_y(Angle_Y,Angle_expect_y);

	
	mpu6050_send_data(x_PWM_High,x,y,Angle_expect_x,Angle_expect_y,inc_pwm_x);
	
	if(inc_pwm_x > 0)
	{
		x_PWM_High = 5000 + inc_pwm_x;
		x_PWM_Low  = 5000 - inc_pwm_x;
	}
	else
	{
		x_PWM_Low  = 5000 + inc_pwm_x;
		x_PWM_High = 5000 - inc_pwm_x;
	}
	if(x_PWM_High >= 10000)
	{
		x_PWM_High = 10000;
	}
	if(x_PWM_Low <= 0)
	{
		x_PWM_Low = 0;
	}
	
	if(inc_pwm_y > 0)
	{	
		y_PWM_High = 5000 + inc_pwm_y;
		y_PWM_Low  = 5000 - inc_pwm_y;
	}
	else
	{
		y_PWM_Low  = 5000 + inc_pwm_y;
		y_PWM_High = 5000 - inc_pwm_y;
	}
	if(y_PWM_High >= 10000)
	{
		y_PWM_High = 10000;
	}
	if(y_PWM_Low <= 0)
	{
		y_PWM_Low = 0;
	}

	if(inc_pwm_x > 0)
	{
		pwm2 = x_PWM_Low;
		pwm6 = x_PWM_Low;
		pwm3 = x_PWM_High;
		pwm7 = x_PWM_High;
	}
	else
	{
		pwm2 = x_PWM_High;
		pwm6 = x_PWM_High;
		pwm3 = x_PWM_Low;
		pwm7 = x_PWM_Low;
	}
	
	if(inc_pwm_y > 0)
	{	
		pwm0 = y_PWM_Low;
		pwm4 = y_PWM_Low;
		pwm1 = y_PWM_High;
		pwm5 = y_PWM_High;
	}
	else
	{
		pwm0 = y_PWM_High;
		pwm4 = y_PWM_High;
		pwm1 = y_PWM_Low;
		pwm5 = y_PWM_Low;
	}
	SetPwm(pwm0,pwm1,pwm2,pwm3,pwm4,pwm5,pwm6,pwm7);
	
	if(nowtime >= bgT)
	{
		nowtime=0;
	}

}
