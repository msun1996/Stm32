#include "imu.h"
#include "math.h"

float Accel_x;	     //X����ٶ�ֵ�ݴ�
float Accel_y;	     //Y����ٶ�ֵ�ݴ�
float Accel_z;	     //Z����ٶ�ֵ�ݴ�

float Gyro_x;		 //X�������������ݴ�
float Gyro_y;    //Y�������������ݴ�
float Gyro_z;		 //Z�������������ݴ�

//float Angle_gy;    //�ɽ��ٶȼ������б�Ƕ�
float Angle_x_temp;  //�ɼ��ٶȼ����x��б�Ƕ�
float Angle_y_temp;  //�ɼ��ٶȼ����y��б�Ƕ�
float Angle_z_temp;

float Angle_X_Final; //X������б�Ƕ�
float Angle_Y_Final; //Y������б�Ƕ�
float Angle_Z_Final; //Z������б�Ƕ�

//����������
char  C_0 = 1;
float Q_bias_x, Q_bias_y, Q_bias_z;
float Angle_err_x, Angle_err_y, Angle_err_z;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = { 0,0,0,0 };
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

//�������궨����
float Angle_X_1_sum;//�����νǶȺ�
float Angle_Y_1_sum;
float Angle_Z_1_sum;

float Angle_X_1; //�����νǶ�ƽ��
float Angle_Y_1; 
float Angle_Z_1;

float Angle_X_2_sum; //ż���νǶ�
float Angle_Y_2_sum; 
float Angle_Z_2_sum; 

float Angle_X_2; //ż���Ƕ�ƽ��
float Angle_Y_2; 
float Angle_Z_2; 

float Angle_X_static; //��̬��ǲο�ֵ
float Angle_Y_static; 
float Angle_Z_static;

float Angle_X; //�������궨��Ƕ�
float Angle_Y; 
float Angle_Z;

char Xstaticflag=0; //X��궨��־
char Ystaticflag=0; //Y��궨��־
char Zstaticflag=0; //Z��궨��־
char staticflag=0; 
int st;

double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R)
{
	double R = MeasureNoise_R;
	double Q = ProcessNiose_Q;
	static double x_last;
	double x_mid = x_last;
	double x_now;
	static double p_last;
	double p_mid;
	double p_now;
	double kg;
	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kg=kalman filter,R=����
	x_now = x_mid + kg*(ResrcData - x_mid);//������ֵ
	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance
	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬
	return x_now;
}

//�Ƕȼ���
void Angle_Calcu(void)
{
	//��ΧΪ2gʱ�������ϵ��16384 LSB/g
	//deg = rad*180/3.14
	float x=0, y=0, z=0;

	Accel_x = aacx; //x����ٶ�ֵ�ݴ�
	Accel_y = aacy; //y����ٶ�ֵ�ݴ�
	Accel_z = aacz; //z����ٶ�ֵ�ݴ�
	Gyro_x = gyrox;  //x��������ֵ�ݴ�
	Gyro_y = gyroy;  //y��������ֵ�ݴ�
	Gyro_z = gyroz;  //z��������ֵ�ݴ�

	//����x����ٶ�
	if (Accel_x<32764) x = Accel_x / 16384;
	else              x = 1 - (Accel_x - 49152) / 16384;

	//����y����ٶ�
	if (Accel_y<32764) y = Accel_y / 16384;
	else              y = 1 - (Accel_y - 49152) / 16384;

	//����z����ٶ�
	if (Accel_z<32764) z = Accel_z / 16384;
	else              z = (Accel_z - 49152) / 16384;

	//�ü��ٶȼ����������ˮƽ������ϵ֮��ļн�
	Angle_x_temp = (atan2(z , y)) * 180 / Pi;
	Angle_y_temp = (atan2(x , z)) * 180 / Pi;
	Angle_z_temp = (atan2(y , x)) * 180 / Pi;

	//�Ƕȵ�������
	if (Accel_y<32764) Angle_y_temp = +Angle_y_temp;
	if (Accel_y>32764) Angle_y_temp = -Angle_y_temp;
	if (Accel_x<32764) Angle_x_temp = +Angle_x_temp;
	if (Accel_x>32764) Angle_x_temp = -Angle_x_temp;
	if (Accel_z<32764) Angle_z_temp = +Angle_z_temp;
	if (Accel_z>32764) Angle_z_temp = -Angle_z_temp;
	
	//���ٶ�
	//��ǰ�˶�
	if (Gyro_x<32768) Gyro_x = -(Gyro_x / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_x>32768) Gyro_x = +(65535 - Gyro_x) / 16.4;
	//��ǰ�˶�
	if (Gyro_y<32768) Gyro_y = -(Gyro_y / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_y>32768) Gyro_y = +(65535 - Gyro_y) / 16.4;
	//��ǰ�˶�
	if (Gyro_z<32768) Gyro_z = -(Gyro_z / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_z>32768) Gyro_z = +(65535 - Gyro_z) / 16.4;

	//Angle_gy = Angle_gy + Gyro_y*0.025;  //���ٶȻ��ֵõ���б�Ƕ�.Խ����ֳ����ĽǶ�Խ��
	Kalman_Filter_X(Angle_x_temp, Gyro_x);  //�������˲�����X���
//	yijiehubu_P(Angle_x_temp, Gyro_x);
//	Erjielvbo(Angle_x_temp, Gyro_x);
	Kalman_Filter_Y(Angle_y_temp, Gyro_y);  //�������˲�����Y���
	Kalman_Filter_Z(Angle_z_temp, Gyro_z);  //�������˲�����Y���
}

void Kalman_Filter_X(float Accel, float Gyro) //����������
{
	Angle_X_Final += (Gyro - Q_bias_x) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_x = Accel - Angle_X_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_X_Final += K_0 * Angle_err_x;	 //�������
	Q_bias_x += K_1 * Angle_err_x;	 //�������
	Gyro_x = Gyro - Q_bias_x;	 //���ֵ(�������)��΢��=���ٶ�
}

void Kalman_Filter_Y(float Accel, float Gyro) //����������		
{
	Angle_Y_Final += (Gyro - Q_bias_y) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_y = Accel - Angle_Y_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Y_Final += K_0 * Angle_err_y;	 //�������
	Q_bias_y += K_1 * Angle_err_y;	 //�������
	Gyro_y = Gyro - Q_bias_y;	 //���ֵ(�������)��΢��=���ٶ�
}

void Kalman_Filter_Z(float Accel, float Gyro) //����������
{
	Angle_Z_Final += (Gyro - Q_bias_z) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_z = Accel - Angle_Z_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Z_Final += K_0 * Angle_err_z;	 //�������
	Q_bias_z += K_1 * Angle_err_z;	 //�������
	Gyro_z = Gyro - Q_bias_z;	 //���ֵ(�������)��΢��=���ٶ�
}

//���ȡ100������ƽ���Ƚϣ�������ƽ��ֵ�������������ȡ��ֵΪ��̬�궨ֵ
void Kalman_biaoding(void)
{
	st++;
	//X��Ƕȱ궨
	if(Xstaticflag == 0)  
	{
		if(Angle_X_1 >= Angle_X_2-0.05 && Angle_X_1 <= Angle_X_2+0.05 && Angle_X_1 != 0)
		{
			Angle_X_static = Angle_X_Final;
			Xstaticflag=1;
		}
		else
		{
			if(st<=100)
			{
				Angle_X_1_sum += Angle_X_Final;
				if(st==100)
				{
					Angle_X_1 = Angle_X_1_sum/100;
					Angle_X_1_sum = 0; 
				}
			}
			else if(st<=200)
			{
				Angle_X_2_sum += Angle_X_Final;
				if(st==200)
				{
					Angle_X_2 = Angle_X_2_sum/100;
					Angle_X_2_sum = 0;
					st=0;
				}
			}
		}
	}
	Angle_X = Angle_X_Final-Angle_X_static;
	
	// Y��Ƕȱ궨
	if(Ystaticflag == 0)  
	{
		if(Angle_Y_1 >= Angle_Y_2-0.05 && Angle_Y_1 <= Angle_Y_2+0.05 && Angle_Y_1 != 0)
		{
			Angle_Y_static = Angle_Y_Final;
			Ystaticflag=1;
		}
		else
		{
			if(st<=100)
			{
				Angle_Y_1_sum += Angle_Y_Final;
				if(st==100)
				{
					Angle_Y_1 = Angle_Y_1_sum/100;
					Angle_Y_1_sum = 0; 
				}
			}
			else if(st<=200)
			{
				Angle_Y_2_sum += Angle_Y_Final;
				if(st==200)
				{
					Angle_Y_2 = Angle_Y_2_sum/100;
					Angle_Y_2_sum = 0;
					st=0;
				}
			}
		}
	}
	Angle_Y = Angle_Y_Final-Angle_Y_static;
	
	//Z��yaw�����궨
	Angle_Z = Angle_Z_Final;
	
	if(Xstaticflag == 1 && Ystaticflag==1)
	{
		staticflag=1;
	}
}

float angle_P,angle_R;
float A_P,A_R,A2_P;

void yijiehubu_P(float angle_m, float gyro_m)
{
	float K1 =0.09;
	float d=0.01;
	angle_P = K1 * angle_m + (1-K1) * (angle_P + gyro_m * d);
	A_P = angle_P;
}

void yijiehubu_R(float angle_m, float gyro_m)
{
	float K1 =0.1;
	angle_R = K1 * angle_m + (1-K1) * (angle_R + gyro_m * dt);
	A_R = angle_R;
}
