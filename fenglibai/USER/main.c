#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "timer.h"
#include "pos_mach.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
float pitch,roll,yaw; 		//欧拉角
		
void All_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	Stm32_Clock_Init(336,8,2,7); //设置时钟，168Mhz
	uart_init(115200); //串口(蓝牙)初始化，波特率设置115200
	delay_init(168); //延时初始化
	MPU_Init(); //mpu6050初始化
	mpu_dmp_init();
	TIM3_Int_Init(500-1,840-1); //100kHz频率 5ms中断
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
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
			usart1_report_imu(roll*100,pitch*100,yaw*100);
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


