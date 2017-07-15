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


float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����

extern float Angle_x_temp;  //�ɼ��ٶȼ����x��б�Ƕ�
extern float Angle_y_temp;  //�ɼ��ٶȼ����y��б�Ƕ�
extern float Angle_z_temp;

extern float Angle_X; //�������궨��Ƕ�
extern float Angle_Y; 
extern float Angle_Z;

extern char staticflag; //�������궨��ɱ�־λ

u8 ms;

void All_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	Stm32_Clock_Init(336,8,2,7); //����ʱ�ӣ�168Mhz
	uart_init(115200); //����(����)��ʼ��������������115200
	delay_init(168); //��ʱ��ʼ��
	LED_Init();
	MPU_Init(); //mpu6050��ʼ��
	mpu_dmp_init(); //dmp��ʼ��
	TIM3_Int_Init(1000-1,840-1); //100kHzƵ�� 10ms�ж�
	PWM_Out_Init(400); //��ʼ��pwm����400Hz
	SetPwm(5000,5000,5000,5000,5000,5000,5000,5000);
}

void get_angle()
{
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	Angle_Calcu(); //�������˲��㷨
	Kalman_biaoding();
	usart1_report_imu(Angle_X*100,Angle_Y*100,Angle_Z*100);
}

int main(void)
{
	All_init();
	while(1);
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		ms++;
		
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//			Angle_Calcu(); //�������˲��㷨
//			usart1_report_imu(roll*100,-pitch*100,yaw*100);
//			mpu6050_send_data(Angle_x_temp*100,Angle_y_temp*100,Angle_z_temp*100,Angle_X_Final*100,Angle_Y_Final*100,Angle_Z_Final*100);
//		}
		
		get_angle(); //ÿ10ms���п������˲��õ���ǰ��Ƕ�
		
		if(staticflag == 1) //�жϱ궨�Ƿ���ɣ�����ڽ���pid���pwm
		{
			if(ms == 40)
			{
				CTRL_1();
			}
//			if(ms == 60)
//			{
//				SetPwm(0,0,0,0,0,0,0,0);
//			}
			if(ms == 124)
			{
				CTRL_1();
			}
//			if(ms == 140)
//			{
//				SetPwm(0,0,0,0,0,0,0,0);
//			}
			if(ms == 168)
			{
				ms=0;
			}
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


