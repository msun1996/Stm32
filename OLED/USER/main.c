#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);
	delay_init(168);
	OLED_Init();
	OLED_ShowString(0,0,"TIME:",24);
	OLED_ShowString(0,30,"DIST:",24);
	OLED_ShowString(100,0,"S",24);
	OLED_ShowString(100,30,"CM",24);
	OLED_Refresh_Gram();
	while(1);
}

