#include "ctrl.h"
#include "pid.h"
#include "pwm_out.h"
#include "math.h"
#include "pos_mach.h"
#include "math.h"
#include "FILTER.h"

extern short x_now,y_now;
int x_pwm_inc,y_pwm_inc;
u16 x_pwm0,x_pwm1,y_pwm0,y_pwm1;
extern u16 pwmp0,pwmp1,pwmp2,pwmp3;

float x_k,y_k,x_1,y_1,x_2,y_2,x_final,y_final;


void kerman(void)
{
		if(x_now)
		{
			x_k = x_now;
		}
		else
		{
			x_k = x_1;
		}
		if(y_now)
		{
			y_k = y_now;
		}
		else
		{
			y_k = y_1;
		}
		x_2 = x_k - x_1;
		y_2 = y_k - y_1;
		
		x_final = Kalman_Filter_X(x_k,x_2); //卡尔曼函数	
		y_final = Kalman_Filter_Y(y_k,y_2); //卡尔曼函数	

		x_1 = x_k;
		y_1 = y_k;
		
		mpu6050_send_data(x_k,y_k,x_final,y_final,0,0);
}

u32 iii=0;
void Ctrl_1(void)
{
//	iii++;
//	kerman();
//	if(iii>=100)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,47);
//		y_pwm_inc = PID_CAL_y(y_final,19);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//		iii=99;
//	}
	
		iii++;
	kerman();
	if(iii>=100)
	{
		x_pwm_inc = PID_CAL_x(x_final,46);
		y_pwm_inc = PID_CAL_y(y_final,46);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
		iii=99;
	}
}

void Ctrl_2(void)
{
	iii++;
	kerman();
	if(iii>=100)
	{
		x_pwm_inc = PID_CAL_x(x_final,46);
		y_pwm_inc = PID_CAL_y(y_final,45);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
		iii=99;
	}
}
u8 flag3=0;
u8 jj3=0;
void Ctrl_3(void)
{
//	
//	iii++;
//	kerman();
//	if(iii>=100&&flag3==0)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,74);
//		y_pwm_inc = PID_CAL_y(y_final,46);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//		iii=99;
//	}

//	if(x_final<=80 && x_final>=70&&y_final>=40 &&y_final<=51)
//	{
//		jj3++;
//	}
//	else
//	{
//		jj3=0;
//	}
//	if(jj3>=200)
//	{
//		flag3=1;
//		jj3=200;
//	}
//	
//	if(flag3==1)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,46);
//		y_pwm_inc = PID_CAL_y(y_final,45);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//	}
	
	iii++;
	kerman();
	if(iii>=100&&flag3==0)
	{
		x_pwm_inc = PID_CAL_x(x_final,47);
		y_pwm_inc = PID_CAL_y(y_final,74);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
		iii=99;
	
	}

	if(x_final<=55 && x_final>=40&&y_final>=68&&y_final<=80)
	{
		jj3++;
	}
	else
	{
		jj3=0;
	}
	if(jj3>=200)
	{
		flag3=1;
		jj3=200;
	}
	
	if(flag3==1)
	{
		x_pwm_inc = PID_CAL_x(x_final,46);
		y_pwm_inc = PID_CAL_y(y_final,45);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
	}	
}

void Ctrl_4(void)
{
//	iii++;
//	kerman();
//	if(iii>=100&&flag3==0)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,46);
//		y_pwm_inc = PID_CAL_y(y_final,46);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//		iii=99;
//	}

//	if(x_final>=40 && x_final<=50 &&y_final>=40&&y_final<=50)
//	{
//		jj3++;
//	}
//	else
//	{
//		jj3=0;
//	}
//	if(jj3>=100)
//	{
//		flag3=1;
//		jj3=200;
//	}
//	
//	if(flag3==1)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,19);
//		y_pwm_inc = PID_CAL_y(y_final,19);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//	}

	iii++;
	kerman();
	if(iii>=100)
	{
		x_pwm_inc = PID_CAL_x(x_final,19);
		y_pwm_inc = PID_CAL_y(y_final,19);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
		iii=99;
	}
}


u8 flag5_1,flag5_2;
u8 jj5_1,jj5_2;
void Ctrl_5(void)
{
	iii++;
	kerman();
	if(iii>=100&&flag5_1==0)
	{
		x_pwm_inc = PID_CAL_x(x_final,47);
		y_pwm_inc = PID_CAL_y(y_final,74);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
		iii=99;
	}

	if(x_final<=52 && x_final>=44&&y_final>=71 &&y_final<=79)
	{
		jj5_1++;
	}
	else
	{
		jj5_1=0;
	}
	if(jj5_1>=200)
	{
		flag5_1=1;
		jj5_1=200;
	}
		if(x_final<=52 && x_final>=44&&y_final>=16 &&y_final<=22)
	{
		jj5_2++;
	}
	else
	{
		jj5_2=0;
	}
	if(jj5_2>=200)
	{
		flag5_2=1;
		jj5_2=200;
	}
 
	if(flag5_1==1&&flag5_2==0)
	{
		x_pwm_inc = PID_CAL_x(x_final,47);
		y_pwm_inc = PID_CAL_y(y_final,19);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
	}
	if(flag5_1==1&&flag5_2==1)
	{
		x_pwm_inc = PID_CAL_x(x_final,19);
		y_pwm_inc = PID_CAL_y(y_final,19);
		x_pwm0 = pwmp0 - x_pwm_inc;
		x_pwm1 = pwmp2 + x_pwm_inc;
		y_pwm0 = pwmp1 - y_pwm_inc;
		y_pwm1 = pwmp3 + y_pwm_inc;
		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);			
	}
	
}




//u8 flag7=0;
//u8 jj7=0;
//u8 jj7_1;

//float Sin[20] = {0,0.3247,0.6142,0.8372, 0.9694, 0.9966,0.9158,0.7357,0.4759 ,0.1646,-0.1646,-0.4759,-0.7357,-0.9158,-0.9966,-0.9694,-0.8372,-0.6142,-0.3247,0};
//float Cos[20] = {1, 0.9458,0.7891,0.5469,0.2455,-0.0826,-0.4017,-0.6773,-0.8795,-0.9864,-0.9864,-0.8795, -0.6773,-0.4017 ,-0.0826,0.2455,0.5469,0.7891,0.9458,1};
//float Sin_1[10] = {0,0.6428, 0.9848,0.8660, 0.3420,-0.3420,-0.8660,-0.9848,-0.6428,0};
//float Cos_1[10] = {1,0.7660,0.1736,-0.5000,-0.9397,-0.9397,-0.5000,0.1736,0.7660,1};

//float yuan_X;
//float yuan_Y;
//	
//void Ctrl_7(void)
//{
////	const float priod = 1410.0;
////	const float priod = 3000.0;
////	static uint32_t movetimecnt = 0;
////	float set_x = 0.0,set_y = 0.0,Normalization = 0.0,Omega = 0.0;

//	iii++;
//	kerman();
//	if(iii>=100&&flag5_1==0)
//	{
//		x_pwm_inc = PID_CAL_x(x_final,46);
//		y_pwm_inc = PID_CAL_y(y_final,56);
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
//		iii=99;
//	}
//	
//	if(x_final<=49 && x_final>=43&&y_final>=53 &&y_final<=59)
//	{
//		jj7++;
//	}
//	else
//	{
//		jj7=0;
//	}
//	if(jj5_1>=200)
//	{
//		flag7=1;
//		jj7=200;
//	}
//	
//	if(x_final<=49 && x_final>=43&&y_final>=53 &&y_final<=59)
//	{
//		jj7++;
//	}
//	else
//	{
//		jj7=0;
//	}
//	if(jj5_1>=200)
//	{
//		flag7=1;
//		jj7=200;
//	}

//	if(x_final<=49 && x_final>=43&&y_final>=53 &&y_final<=59)
//	{
//		jj7++;
//	}
//	else
//	{
//		jj7=0;
//	}
//	if(jj5_1>=200)
//	{
//		flag7=1;
//		jj7=200;
//	}	
//	if(flag7==1)
//	{		
//		yuan_X = PID_CAL_x(x_final,set_x);
//		yuan_Y = PID_CAL_y(y_final,set_y);
//		x_pwm_inc = yuan_X;
//		y_pwm_inc = yuan_Y;		
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);		
//	}




//	movetimecnt +=10;
//	Normalization = (float)movetimecnt / priod;
//	Omega = 2.0*3.14159*Normalization;

//	x_pwm_inc = 600*sin(Omega);
//	y_pwm_inc = 600*sin(Omega + 1.570795);
//		
//	iii++;
//	if(iii>=100)
//	{
//		iii = 99;
//		yuan_X = 600*Sin[jj7_1];
//		yuan_Y = 600*Cos[jj7_1];
//		x_pwm_inc = yuan_X;
//		y_pwm_inc = yuan_Y;		
//		x_pwm0 = pwmp0 - x_pwm_inc;
//		x_pwm1 = pwmp2 + x_pwm_inc;
//		y_pwm0 = pwmp1 - y_pwm_inc;
//		y_pwm1 = pwmp3 + y_pwm_inc;
//		SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);		
//		jj7_1++;
//		if(jj7_1 == 20)
//			jj7_1 = 0;
//	}
	
//}



float tts=0;
float setx,sety;
void set0(u8 x_cs,u8 x_mb,u8 y_cs,u8 y_mb,float T)
{
	tts++;
	setx = x_cs + (x_mb-x_cs)/T*tts;
	sety = y_cs + (y_mb-y_cs)/T*tts;
	x_pwm_inc = PID_CAL_x(x_final,setx);
	y_pwm_inc = PID_CAL_y(y_final,sety);
	x_pwm0 = pwmp0 - x_pwm_inc;
	x_pwm1 = pwmp2 + x_pwm_inc;
	y_pwm0 = pwmp1 - y_pwm_inc;
	y_pwm1 = pwmp3 + y_pwm_inc;
	SetPwm_dj(x_pwm0,y_pwm0,x_pwm1,y_pwm1);
	if(tts >= T)
	{
		tts--;	
	}
}


void Ctrl_N1(void)
{
	iii++;
	kerman();
	if(iii>=200)
	{

		set0(75,75,46,46,200);
		iii=199;
	}
}

void Ctrl_N2(void)
{
	iii++;
	kerman();
	if(iii>=100)
	{
		set0(75,47,75,47,200);
		iii=99;
	}
}


u8 flagn3=0;
u8 jjn3=0;
void Ctrl_N3(void)
{
	iii++;
	kerman();
	if(iii>=100)
	{
		if(flagn3==0)
		{
			set0(75,47,75,75,200);	
			if(x_final<=52 && x_final>=42 && y_final>=70 &&y_final<=80)
			{
				jjn3++;
			}
			else
			{
				jjn3=0;
			}
			if(jjn3>=250)
			{
				flagn3=1;
				jjn3=200;
				tts=0;
			}
		}
		else
		{
			set0(47,47,75,47,200);
		}
	}
}

void Ctrl_N4(void)
{
	iii++;
	kerman();
	if(iii>=100 && iii<=300)
	{
		set0(75,53,75,39,200);
		if(iii==300)
		{
			tts=0;
		}
	}
	else if(iii> 300)
	{
		set0(53,19,39,19,300);
	}

}

void Ctrl_N5(void)
{
	iii++;
	kerman();
	if(iii>=100 && iii<=300)
	{
		set0(75,73,75,46,200);
		if(iii==300)
		{
			tts=0;
		}
	}
	else if(iii>300 && iii <=400)
	{
		set0(73,73,46,46,100);
		if(iii==400)
		{
			tts=0;
		}
	}
	else if(iii>400 && iii <= 550)
	{
		set0(73,47,46,19,150);
		if(iii==550)
		{
			tts=0;
		}
	}
	else if(iii>550 && iii <=650)
	{
		set0(47,47,19,19,100);
		if(iii == 650)
		{
			tts=0;
		}
	}
	else if(iii>650)
	{
		set0(47,19,19,19,200);
	}
}

extern u8 Ax,Ay,Bx,By,Cx,Cy,Dx,Dy;

void Ctrl_N6(void)
{
	iii++;
	kerman();
	if(iii>=100 && iii<=600)
	{
		set0(Ax,Bx,Ay,By,500);
		if(iii==600)
		{
			tts=0;
		}
	}
	else if(iii>600 && iii <= 1100)
	{
		set0(Bx,Cx,By,Cy,500);
		if(iii==1100)
		{
			tts=0;
		}
	}
	else if(iii>1100)
	{
		set0(Cx,Dx,Cy,Dy,500);
	}
}

void Ctrl_N7(void)
{
	iii++;
	kerman();
	if(iii>=100 && iii<=300)
	{
		set0(75,60,75,60,200);
		if(iii==300)
		{
			tts=0;
		}
	}
	
	else if(iii>300 && iii<=500) 	//第一圈
	{
		set0(60,30,60,60,200);
		if(iii==500)
		{
			tts=0;
		}
	}
	else if(iii>500 && iii<=700)
	{
		set0(30,30,60,30,200);
		if(iii==700)
		{
			tts=0;
		}
	}
	else if(iii>700 && iii<=900)
	{
		set0(30,60,30,30,200);
		if(iii==900)
		{
			tts=0;
		}
	}
	else if(iii>900 && iii<=1100)
	{
		set0(60,60,30,60,200);
		if(iii==1100)
		{
			tts=0;
		}
	}
	
	else if(iii>1100 && iii<=1300) 	//第二圈
	{
		set0(60,30,60,60,200);
		if(iii==1300)
		{
			tts=0;
		}
	}
	else if(iii>1300 && iii<=1500)
	{
		set0(30,30,60,30,200);
		if(iii==1500)
		{
			tts=0;
		}
	}
	else if(iii>1500 && iii<=1700)
	{
		set0(30,60,30,30,200);
		if(iii==1700)
		{
			tts=0;
		}
	}
	else if(iii>1700 && iii<=1900)
	{
		set0(60,60,30,60,200);
		if(iii==1900)
		{
			tts=0;
		}
	}
	
	else if(iii>1900 && iii<=2100) 	//第三圈
	{
		set0(60,30,60,60,200);
		if(iii==2100)
		{
			tts=0;
		}
	}
	else if(iii>2100 && iii<=2300)
	{
		set0(30,30,60,30,200);
		if(iii==2300)
		{
			tts=0;
		}
	}
	else if(iii>2300 && iii<=2500)
	{
		set0(30,60,30,30,200);
		if(iii==2500)
		{
			tts=0;
		}
	}
	else if(iii>2500 && iii<=2700)
	{
		set0(60,60,30,60,200);
		if(iii==2700)
		{
			tts=0;
		}
	}
	
	else if(iii>2700 && iii<=2900) 	//第四圈
	{
		set0(60,30,60,60,200);
		if(iii==2900)
		{
			tts=0;
		}
	}
	else if(iii>2900 && iii<=3100)
	{
		set0(30,30,60,30,200);
		if(iii==3100)
		{
			tts=0;
		}
	}
	
	else if(iii>3100 && iii<=3300) 	//四圈半到9点
	{
		set0(30,19,30,19,200);
		if(iii==3300)
		{
			tts=0;
		}
	}
}
