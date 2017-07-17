#include "pid.h"
#include "math.h"

float Kp=80;
float Ki=15;
float Kd=15;

float err_x_Angle; //当前误差
float err_x_Angle_last; //上次误差
float err_x_Angle_last_next; //上上次误差

float err_y_Angle; //当前误差
float err_y_Angle_last; //上次误差
float err_y_Angle_last_next; //上上次误差

short Kp_err;
short Ki_err;
short Kd_err;

short inc_x_PWM;
short inc_y_PWM;


//PID计算，输入现实倾角和期望倾角，输出pwm变化量
short PID_CAL_x(float Angle_X_now,float Angle_x_except)
{
	err_x_Angle = Angle_x_except - fabs(Angle_X_now);
	Kp_err = err_x_Angle-err_x_Angle_last;
	Ki_err = err_x_Angle;
	Kd_err = err_x_Angle-2*err_x_Angle_last+err_x_Angle_last_next;
	inc_x_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
	err_x_Angle_last_next = err_x_Angle_last;
	err_x_Angle_last = err_x_Angle;
	
	return inc_x_PWM;
}

short PID_CAL_y(float Angle_Y_now,float Angle_y_except)
{
	err_y_Angle = Angle_y_except - fabs(Angle_Y_now);
	Kp_err = err_y_Angle-err_y_Angle_last;
	Ki_err = err_y_Angle;
	Kd_err = err_y_Angle-2*err_y_Angle_last+err_y_Angle_last_next;
	inc_y_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
	err_y_Angle_last_next = err_y_Angle_last;
	err_y_Angle_last = err_y_Angle;
	
	return inc_y_PWM;
}


