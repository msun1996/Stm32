#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "pwm_out.h"
#include "pos_mach.h"
#include "led.h"
#include "oled.h"
#include "ctrl.h"
#include "key.h"
#include "csb.h"
#include "stepmotor.h"

u8 mode;
u8 keyvalue;
char modelast=100;
u16 location=1;

extern float length;

void mode_select(void)
{
	mode=DIP_scan();
	keyvalue=KEY_scan();
	if(keyvalue == 2)
	{
		location +=1;
	}
	else if(keyvalue == 4)
	{  
		location -=1;
	}
	if(mode != modelast)
	{
		modelast = mode;
		OLED_Clear();
	}
	if(mode == 0)
	{
		OLED_ShowString(0,0,"mode0",16);
		OLED_Showhzstring(0,16,"位置");
		OLED_ShowString(32,16,":",16);
		OLED_ShowNum(50,16,location,5,16);
		OLED_ShowString(100,16,"CM",16);
		OLED_Refresh_Gram();
	}
	else if(mode == 1)
	{
		OLED_ShowString(0,0,"mode1",16);
		OLED_Showhzstring(0,16,"位置");
		OLED_ShowString(32,16,":",16);
		OLED_ShowString(100,16,"CM",16);
		OLED_Showhzstring(0,32,"半径");
		OLED_ShowString(32,32,":",16);
		OLED_ShowString(100,32,"CM",16);
		OLED_Showhzstring(0,48,"周期");
		OLED_ShowString(32,48,":",16);
		OLED_ShowString(100,48,"S",16);
		OLED_Refresh_Gram();
	}
	else if(mode == 2)
	{
		OLED_ShowString(0,0,"mode2",16);
		OLED_Showhzstring(0,16,"位置");
		OLED_ShowString(32,16,":",16);
		OLED_ShowString(100,16,"CM",16);
		OLED_Showhzstring(0,32,"半径");
		OLED_ShowString(32,32,":",16);
		OLED_ShowString(100,32,"CM",16);
		OLED_Showhzstring(0,48,"周期");
		OLED_ShowString(32,48,":",16);
		OLED_ShowString(100,48,"S",16);
		OLED_Refresh_Gram();
	}
}

void All_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	Stm32_Clock_Init(336,8,2,7); //设置时钟，168Mhz
	uart_init(115200); //串口(蓝牙)初始化，波特率设置115200
	delay_init(168); //延时初始化
	LED_Init();
	KEY_Init();
	OLED_Init();
	stepmotor_init();
	Step_PWM_init();
	CH_SR04_Init();
	TIM3_Int_Init(1000-1,840-1); //100kHz频率 10ms中断
}

int main(void)
{
	All_init();
	
	OLED_Showhzstring(20,16,"小球控制系统");
	OLED_Refresh_Gram();
	delay_ms(2000);
	while(1)
	{
		mode_select();
	}
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		step_PWM(location);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		data=USART_ReceiveData(USART1);	//读取接收到的数据
		usart1_receive_char(data);
	}
}

