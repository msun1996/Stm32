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


float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据

extern float Angle_x_temp;  //由加速度计算的x倾斜角度
extern float Angle_y_temp;  //由加速度计算的y倾斜角度
extern float Angle_z_temp;

extern float Angle_X; //卡尔曼标定后角度
extern float Angle_Y; 
extern float Angle_Z;

extern char staticflag; //标定完成标志位

s16 pwm_out[8]; //定义8pwm波输入值

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
	//PWM_Out_Init(400); //初始化pwm波，400Hz
	/* pwm波调用
	pwm[0] = 1000;
	pwm[1] = 1000;
	SetPwm(pwm_out);
	*/
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
		
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		Angle_Calcu(); //卡尔曼滤波算法
		Kalman_biaoding();
		if(staticflag == 1)
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		}
		usart1_report_imu(Angle_X*100,Angle_Y*100,Angle_Z*100);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


