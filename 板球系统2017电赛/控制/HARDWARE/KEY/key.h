#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define DIP1 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)  //PD8
#define KEY4 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)	 //PD9
#define DIP2 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) //PD10
#define KEY1 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) //PD11

#define DIP3 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) //PD12
#define KEY3 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) //PD13
#define DIP4 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) //PD14
#define KEY2 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) //PD15



void KEY_Init(void);	//IO初始化
u8 DIP_scan(void);
u8 KEY_scan(void);

#endif
