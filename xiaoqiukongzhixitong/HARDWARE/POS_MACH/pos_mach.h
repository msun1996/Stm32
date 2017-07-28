#ifndef __POS_MACH_H_
#define __POS_MACH_H_

#include "stm32f4xx.h"

//单片机向上位机传输数据
void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8 *data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short roll,short pitch,short yaw);
void Data_send_MotoPWM(short MotoPWM1,short MotoPWM2,short MotoPWM3,short MotoPWM4);

void send_receive_check(u8 head,u8 check_sum);//单片机反馈检查

//上位机向单片机写指令
void usart1_receive_char(u8 data);
void Data_Receive_Anl(u8 *data,u8 num);

#endif
