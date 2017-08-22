#ifndef _PID_H
#define _PID_H

#include "stm32f4xx.h"

//float PID_CAL(float location_now,float location_expect);
float PID_CAL_x(float Angle_X_now,float Angle_x_except);
float PID_CAL_y(float Angle_Y_now,float Angle_y_except);

#endif
