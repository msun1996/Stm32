
#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

#define Pi 	3.1415926
//卡尔曼噪声参数
#define dt           0.010
#define R_angle      0.5 //上次角置信度，比下面越大，开始调节时间越长
#define Q_angle      0.001 //角度置信度 波形质量下降，但跟随性更好
#define Q_gyro       0.005 //角速度置信度 越大越滞后

//引用外部变量
extern short aacx, aacy, aacz;		//加速度传感器原始数据
extern short gyrox, gyroy, gyroz;	//陀螺仪原始数据

double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);
void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Kalman_Filter_Z(float Accel, float Gyro);
void Kalman_biaoding();
void yijiehubu_P(float angle_m, float gyro_m);

#endif
