#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "imu.h"
#include "timer.h"
#include "pwm_out.h"
#include "pos_mach.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "led.h"
#include "ctrl.h"


float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据

extern float Angle_x_temp;  //由加速度计算的x倾斜角度
extern float Angle_y_temp;  //由加速度计算的y倾斜角度
extern float Angle_z_temp;

extern float Angle_X;//卡尔曼标定后角度
extern float Angle_Y; 
extern float Angle_Z;

extern char staticflag; //卡尔曼标定完成标志位

u8 ms1,ms2;

u8 mode=0; //方式选择
float length_any; //长度（半径）
u8 angle_any; //角度

extern float Ki_err_x;
extern float Ki_err_y;

u8 startflag;

extern float err_x_Angle; //当前误差
extern float err_x_Angle_last; //上次误差
extern float err_x_Angle_last_next; //上上次误差

extern float err_y_Angle; //当前误差
extern float err_y_Angle_last; //上次误差
extern float err_y_Angle_last_next; //上上次误差

extern float x_PWM_High; //X方向PWM波高输出
extern float x_PWM_Low;  //X方向PWM波低输出

extern float y_PWM_High; //Y方向PWM波高输出
extern float y_PWM_Low;  //Y方向PWM波低输出

void All_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	Stm32_Clock_Init(336,8,2,7); //设置时钟，168Mhz
	uart_init(115200); //串口(蓝牙)初始化，波特率设置115200
	delay_init(168); //延时初始化
	LED_Init();
	MPU_Init(); //mpu6050初始化
	mpu_dmp_init(); //dmp初始化
	TIM3_Int_Init(1000-1,840-1); //100kHz频率 10ms中断
	PWM_Out_Init(400); //初始化pwm波，400Hz
	SetPwm(5000,5000,5000,5000,5000,5000,5000,5000);
}

void get_angle()
{
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
	Angle_Calcu(); //卡尔曼滤波算法
	Kalman_biaoding();
	usart1_report_imu(Angle_X*100,Angle_Y*100,Angle_Z*100);
}

int main(void)
{
	All_init();
	while(1);
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			Angle_Calcu(); //卡尔曼滤波算法
//			usart1_report_imu(roll*100,-pitch*100,yaw*100);
//			mpu6050_send_data(Angle_x_temp*100,Angle_y_temp*100,Angle_z_temp*100,Angle_X_Final*100,Angle_Y_Final*100,Angle_Z_Final*100);
//		}
		
		get_angle(); //每10ms进行卡尔曼滤波得到当前倾角度
		
		if(staticflag == 1) //判断标定是否完成，完成在进行pid输出pwm
		{	
			if(mode == 0)
			{
				SetPwm(0,0,0,0,0,0,0,0);
				startflag=0;
				ms1=0;
				ms2=0;
				err_x_Angle = 0 ; //当前误差
				err_x_Angle_last = 0; //上次误差
				err_x_Angle_last_next = 0; //上上次误差

				err_y_Angle = 0; //当前误差
				err_y_Angle_last = 0; //上次误差
				err_y_Angle_last_next = 0; //上上次误差
				
				Ki_err_x=0;
				Ki_err_y=0;
			}
			if(mode == 1)
			{
				way1(angle_any,length_any);
			}
			if(mode == 2)
			{
				way2(length_any);
			}
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		data=USART_ReceiveData(USART1);	//读取接收到的数据
		usart1_receive_char(data);
	}
}
