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

extern float Angle_X;//�������궨��Ƕ�
extern float Angle_Y; 
extern float Angle_Z;

extern char staticflag; //�������궨��ɱ�־λ

u8 ms1,ms2;

u8 mode=0; //��ʽѡ��
float length_any; //���ȣ��뾶��
u8 angle_any; //�Ƕ�

extern float Ki_err_x;
extern float Ki_err_y;

u8 startflag;

extern float err_x_Angle; //��ǰ���
extern float err_x_Angle_last; //�ϴ����
extern float err_x_Angle_last_next; //���ϴ����

extern float err_y_Angle; //��ǰ���
extern float err_y_Angle_last; //�ϴ����
extern float err_y_Angle_last_next; //���ϴ����

extern float x_PWM_High; //X����PWM�������
extern float x_PWM_Low;  //X����PWM�������

extern float y_PWM_High; //Y����PWM�������
extern float y_PWM_Low;  //Y����PWM�������

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
				startflag=0;
				ms1=0;
				ms2=0;
				err_x_Angle = 0 ; //��ǰ���
				err_x_Angle_last = 0; //�ϴ����
				err_x_Angle_last_next = 0; //���ϴ����

				err_y_Angle = 0; //��ǰ���
				err_y_Angle_last = 0; //�ϴ����
				err_y_Angle_last_next = 0; //���ϴ����
				
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
