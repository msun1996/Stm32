#include "pid.h"
#include "math.h"

//float Kp_x=300;
//float Ki_x=0;
//float Kd_x=2500;

//float Kp_y=290;
//float Ki_y=0;
//float Kd_y=3000;

//float err_x_Angle; //当前误差
//float err_x_Angle_last; //上次误差
//float err_x_Angle_last_next; //上上次误差

//float err_y_Angle; //当前误差
//float err_y_Angle_last; //上次误差
//float err_y_Angle_last_next; //上上次误差

//float Kp_err_x;
//float Ki_err_x;
//float Kd_err_x;

//float Kp_err_y;
//float Ki_err_y;
//float Kd_err_y;

//float inc_x_PWM;
//float inc_y_PWM;


//PID计算，输入现实倾角和期望倾角，输出pwm变化量
//float PID_CAL_x(float Angle_X_now,float Angle_x_expect)
//{

//	err_x_Angle = Angle_x_except - Angle_X_now;		
//	Kp_err = err_x_Angle-err_x_Angle_last;
//	Ki_err = err_x_Angle;
//	Kd_err = err_x_Angle-2*err_x_Angle_last+err_x_Angle_last_next;
//	inc_x_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
//	err_x_Angle_last_next = err_x_Angle_last;
//	err_x_Angle_last = err_x_Angle;
//	return inc_x_PWM;
//	
//  位置式pid	
//	err_x_Angle = Angle_x_expect - Angle_X_now;
//	
//	Kp_err_x = err_x_Angle;
//	Ki_err_x = Ki_err_x + err_x_Angle;
//	Kd_err_x = err_x_Angle - err_x_Angle_last;
//	inc_x_PWM = Kp_x*Kp_err_x + Ki_x*Ki_err_x + Kd_x*Kd_err_x;
//	err_x_Angle_last = err_x_Angle;
//	
//	return inc_x_PWM;  
//	
//}

//float PID_CAL_y(float Angle_Y_now,float Angle_y_expect)
//{
//	err_y_Angle = Angle_y_except - Angle_Y_now;		
//	Kp_err = err_y_Angle-err_y_Angle_last;
//	Ki_err = err_y_Angle;
//	Kd_err = err_y_Angle-2*err_y_Angle_last+err_y_Angle_last_next;
//	inc_y_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
//	err_y_Angle_last_next = err_y_Angle_last;
//	err_y_Angle_last = err_y_Angle;
//	
//	return inc_y_PWM;
//	
//  位置式pid	
//	err_y_Angle = Angle_y_expect - Angle_Y_now;

//	Kp_err_y = err_y_Angle;
//	Ki_err_y = Ki_err_y + err_y_Angle;
//	Kd_err_y = err_y_Angle - err_y_Angle_last;
//	inc_y_PWM = Kp_y*Kp_err_y + Ki_y*Ki_err_y + Kd_y*Kd_err_y;
//	err_y_Angle_last = err_y_Angle;
//	
//	return inc_y_PWM;
//}


float Kp;
float Ki;
float Kd;
float err_distant;
float err_distant_last;
float Kp_err;
float Ki_err;
float Kd_err;
float inc;

float PID_CAL(float location_now,float location_expect)
{
	err_distant = location_expect - location_now;
	Kp_err = err_distant;
	Ki_err = Ki_err + err_distant;
	Kd_err = err_distant - err_distant_last;
	
	inc = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
	
	err_distant_last = err_distant;
	return inc;
}
