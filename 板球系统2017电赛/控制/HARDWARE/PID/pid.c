#include "pid.h"
#include "math.h"
#include "FUZZY_PID.h"
#include "pos_mach.h"

//float Kp=0.2;
//float Ki;
//float Kd=2;
//float err_distant;
//float err_distant_last;
//float Kp_err;
//float Ki_err;
//float Kd_err;
//float inc;

//float PID_CAL(float location_now,float location_expect)
//{
//	err_distant = location_expect - location_now;
//	Kp_err = err_distant;
//	Ki_err = Ki_err + err_distant;
//	Kd_err = err_distant - err_distant_last;
//	
//	inc = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
//	
//	err_distant_last = err_distant;
//	return inc;
//}


//float Kp_x=22;
//float Ki_x=0;
//float Kd_x=1600;

////float Kp_y=21.5;
////float Ki_y=0;
////float Kd_y=1800;

//float Kp_y=18;
//float Ki_y=0;
//float Kd_y=1500;

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


////PID计算，输入期望x坐标，输出pwm变化量

//float PID_CAL_x(float Angle_X_now,float Angle_x_expect)
//{
//	//位置式pid	
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
//	
//  //位置式pid	
//	err_y_Angle = Angle_y_expect - Angle_Y_now;

//	Kp_err_y = err_y_Angle;
//	Ki_err_y = Ki_err_y + err_y_Angle;
//	Kd_err_y = err_y_Angle - err_y_Angle_last;
//	inc_y_PWM = Kp_y*Kp_err_y + Ki_y*Ki_err_y + Kd_y*Kd_err_y;
//	err_y_Angle_last = err_y_Angle;
//	
//	return inc_y_PWM;
//}
































float Kp_x=22;
float Ki_x=0;
float Kd_x=1600;

float Kp_y=26;
float Ki_y=0;
float Kd_y=2000;

//float Kp_x=26;
//float Ki_x=0;
//float Kd_x=1800;

//float Kp_y=26;
//float Ki_y=0;
//float Kd_y=2000;

float err_x_Angle; //当前误差
float err_x_Angle_last; //上次误差
float err_x_Angle_last_next; //上上次误差

float err_y_Angle; //当前误差
float err_y_Angle_last; //上次误差
float err_y_Angle_last_next; //上上次误差

float Kp_err_x;
float Ki_err_x;
float Kd_err_x;

float Kp_err_y;
float Ki_err_y;
float Kd_err_y;

float inc_x_PWM;
float inc_y_PWM;


mohu_out mohu_x;
mohu_out mohu_y;

//PID计算，输入期望x坐标，输出pwm变化量

float PID_CAL_x(float Angle_X_now,float Angle_x_expect)
{
//	//位置式pid	
//	err_x_Angle = Angle_x_expect - Angle_X_now;
//	
//	Kp_err_x = err_x_Angle;
//	Ki_err_x = Ki_err_x + err_x_Angle;
//	Kd_err_x = err_x_Angle - err_x_Angle_last;
//	
//	mohu_x = fuzzy(Kp_err_x,Kd_err_x);
//	
//	inc_x_PWM = (Kp_x + mohu_x.p)*Kp_err_x + Ki_x*Ki_err_x + (Kd_x + mohu_x.d)*Kd_err_x;
//	err_x_Angle_last = err_x_Angle;
//	
//	return inc_x_PWM;  
	
	//	//位置式pid	
	err_x_Angle = Angle_x_expect - Angle_X_now;
	
	Kp_err_x = err_x_Angle;
	Ki_err_x = Ki_err_x + err_x_Angle;
	Kd_err_x = err_x_Angle - err_x_Angle_last;
	inc_x_PWM = Kp_x*Kp_err_x + Ki_x*Ki_err_x + Kd_x*Kd_err_x;
	err_x_Angle_last = err_x_Angle;
	
	return inc_x_PWM;  
	
}

float PID_CAL_y(float Angle_Y_now,float Angle_y_expect)
{
	
//  //位置式pid	
//	err_y_Angle = Angle_y_expect - Angle_Y_now;

//	Kp_err_y = err_y_Angle;
//	Ki_err_y = Ki_err_y + err_y_Angle;
//	Kd_err_y = err_y_Angle - err_y_Angle_last;
//	
//	mohu_x = fuzzy(Kp_err_y,Kd_err_y);
//		
//	inc_y_PWM = (Kp_y + mohu_y.p)*Kp_err_y + Ki_y*Ki_err_y + (Kd_y + mohu_y.d)*Kd_err_y;
//	err_y_Angle_last = err_y_Angle;
//	
//	return inc_y_PWM;
	
  //位置式pid	
	err_y_Angle = Angle_y_expect - Angle_Y_now;

	Kp_err_y = err_y_Angle;
	Ki_err_y = Ki_err_y + err_y_Angle;
	Kd_err_y = err_y_Angle - err_y_Angle_last;
	inc_y_PWM = Kp_y*Kp_err_y + Ki_y*Ki_err_y + Kd_y*Kd_err_y;
	err_y_Angle_last = err_y_Angle;
	
	return inc_y_PWM;	
}


