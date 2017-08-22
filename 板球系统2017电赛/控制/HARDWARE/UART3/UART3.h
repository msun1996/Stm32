#ifndef __UART3_H
#define __UART3_H

#include "stm32f4xx.h"

void uart3_init(u32 bound);
void usart3_send_char(u8 c);

void usart3_niming_report(u8 *data, u8 len);
void usart3_send_data(short data1, short data2, short data3);
void usart3_receive_char(u8 data);
void Data_Receive_Anl3(u8 *data_buf,u8 num);
#endif
