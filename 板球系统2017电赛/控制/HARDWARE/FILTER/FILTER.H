#ifndef _FILTER_H
#define	_FILTER_H

#include "sys.h"

float filter_huadong(float data);
float filter_huadong1(float data);

float filter_guanxing(float data);
float Kalman_Filter_X(float Accel, float Gyro); //¿¨¶ûÂüº¯Êı		
float Kalman_Filter_Y(float Accel, float Gyro); //¿¨¶ûÂüº¯Êı		
float Kalman_Filter_UX(float Accel, float Gyro); //¿¨¶ûÂüº¯Êı		
float Kalman_Filter_UY(float Accel, float Gyro); //¿¨¶ûÂüº¯Êı		

#endif

