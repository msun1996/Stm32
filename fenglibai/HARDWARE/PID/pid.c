#include "pid.h"
#include "math.h"

float Kp_x=0;
float Ki_x=0;
float Kd_x=0;

float Kp_y=0;
float Ki_y=0;
float Kd_y=0;


float err_x_Angle; //��ǰ���
float err_x_Angle_last; //�ϴ����
float err_x_Angle_last_next; //���ϴ����

float err_y_Angle; //��ǰ���
float err_y_Angle_last; //�ϴ����
float err_y_Angle_last_next; //���ϴ����

float Kp_err_x;
float Ki_err_x;
float Kd_err_x;

float Kp_err_y;
float Ki_err_y;
float Kd_err_y;

float inc_x_PWM;
float inc_y_PWM;


//PID���㣬������ʵ��Ǻ�������ǣ����pwm�仯��
float PID_CAL_x(float Angle_X_now,float Angle_x_expect)
{
//	if(Angle_x_except >= 0)
//	{
//		err_x_Angle = Angle_x_except - Angle_X_now;
//	}
//	else
//	{
//		err_x_Angle = Angle_X_now - Angle_x_except;		
//	}
//	Kp_err = err_x_Angle-err_x_Angle_last;
//	Ki_err = err_x_Angle;
//	Kd_err = err_x_Angle-2*err_x_Angle_last+err_x_Angle_last_next;
//	inc_x_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
//	err_x_Angle_last_next = err_x_Angle_last;
//	err_x_Angle_last = err_x_Angle;
//	return inc_x_PWM;
	
	
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
//	if(Angle_y_except >= 0)
//	{
//		err_y_Angle = Angle_y_except - Angle_Y_now;
//	}
//	else
//	{
//		err_y_Angle = Angle_Y_now - Angle_y_except;		
//	}
//	Kp_err = err_y_Angle-err_y_Angle_last;
//	Ki_err = err_y_Angle;
//	Kd_err = err_y_Angle-2*err_y_Angle_last+err_y_Angle_last_next;
//	inc_y_PWM = Kp*Kp_err + Ki*Ki_err + Kd*Kd_err;
//	err_y_Angle_last_next = err_y_Angle_last;
//	err_y_Angle_last = err_y_Angle;
//	
//	return inc_y_PWM;
	
	
	err_y_Angle = Angle_y_expect - Angle_Y_now;

	Kp_err_y = err_y_Angle;
	Ki_err_y = Ki_err_y + err_y_Angle;
	Kd_err_y = err_y_Angle - err_y_Angle_last;
	inc_y_PWM = Kp_y*Kp_err_y + Ki_y*Ki_err_y + Kd_y*Kd_err_y;
	err_y_Angle_last = err_y_Angle;
	
	return inc_y_PWM;
}


