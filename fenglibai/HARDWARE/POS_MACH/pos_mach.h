#ifndef __POS_MACH_H_
#define __POS_MACH_H_

#include "stm32f4xx.h"

//单片机向上位机传输数据
void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8 *data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short roll,short pitch,short yaw);

#endif
