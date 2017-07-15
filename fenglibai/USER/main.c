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

u8 ms1,ms2;

u8 mode; //��ʽѡ��
float length_any; //���ȣ��뾶��
u8 angle_any; //�Ƕ�

u8 startflag;

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
	//usart1_report_imu(Angle_X*100,Angle_Y*100,Angle_Z*100);
}
void way1(void)
{
	ms1++;
	if(startflag==0)
	{
		if(angle_any>=90)
		{
			SetPwm(5000,0,5000,0,5000,0,5000,0);
		}
		else
		{
			SetPwm(0,5000,5000,0,0,5000,5000,0);
		}
		startflag=1;
	}
	if(ms1 == 42)
	{
		line(angle_any,length_any);
	}
	if(ms1 == 124)
	{
		line(angle_any,length_any);
	}
	if(ms1 == 168)
	{
		ms1=0;
	}
}
void way2()
{
	ms2++;
	if(ms2 == 42)
	{
		CTRL_x(0.3);
	}
	if(ms2 == 84)
	{
		CTRL_y(0.3);
	}
	if(ms2 == 124)
	{
		CTRL_x(0.3);
	}
	if(ms2 == 168)
	{
		CTRL_y(0.3);
		ms2=0;
	}
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
			if(mode == 0)
			{
				SetPwm(0,0,0,0,0,0,0,0);
			}
			if(mode == 1)
			{
				way1();
			}
			if(mode == 2)
			{
				way2();
			}
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		data=USART_ReceiveData(USART1);	//��ȡ���յ�������
		usart1_receive_char(data);
		
	}	
}
