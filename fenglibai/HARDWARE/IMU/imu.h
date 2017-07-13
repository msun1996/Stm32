
#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#define Pi 	3.1415926
//��������������
#define dt           0.010
#define R_angle      0.5 //�ϴν����Ŷȣ�������Խ�󣬿�ʼ����ʱ��Խ��
#define Q_angle      0.001 //�Ƕ����Ŷ� ���������½����������Ը���
#define Q_gyro       0.005 //���ٶ����Ŷ� Խ��Խ�ͺ�

//�����ⲿ����
extern short aacx, aacy, aacz;		//���ٶȴ�����ԭʼ����
extern short gyrox, gyroy, gyroz;	//������ԭʼ����

double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);
void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Kalman_Filter_Z(float Accel, float Gyro);
void Kalman_biaoding();
void yijiehubu_P(float angle_m, float gyro_m);

#endif
