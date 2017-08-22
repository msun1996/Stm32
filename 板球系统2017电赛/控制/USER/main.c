#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "pwm_out.h"
#include "pos_mach.h"
#include "led.h"
#include "oled.h"
#include "key.h"
#include "UART2.h"
#include "UART3.h"
#include "ctrl.h"
#include "FILTER.h"


u8 mode;
u8 keyvalue;
char modelast=-1;

short x_now,y_now; //x,y轴实时坐标
extern float x_k,y_k,x_1,y_1,x_2,y_2,x_final,y_final;
u16 pwmp0=1350,pwmp1=150,pwmp2=1650,pwmp3=1350;
extern u16 iii;
extern u16 tts;

u8 A6,B6,C6,D6,I6=0,S6=0;
u8 Ax,Ay,Bx,By,Cx,Cy,Dx,Dy;
u8 mode6flag=0;

void self_check(void)
{
	OLED_Clear();
	if(x_now==0 && y_now==0)
	{
		OLED_Showhzstring(20,20,"硬件自检成功");
		OLED_Refresh_Gram();
	}
	else
	{
		OLED_Showhzstring(20,10,"硬件自检失败");
		OLED_ShowString(20,27,"xnow:",16);
		OLED_ShowString(20,44,"ynow:",16);
		OLED_ShowNum(50,27,x_now,3,16);
		OLED_ShowNum(50,44,y_now,3,16);
		OLED_Refresh_Gram();
	}
	delay_ms(1000);
}

void mode6_set(void)
{
	OLED_ShowString(0,0,"SETTING:",16);
	OLED_ShowNum(76,0,S6,2,16);	
	OLED_ShowString(0,16,"A:",16);
	OLED_ShowNum(16,16,A6,2,16);
	OLED_ShowString(0,32,"B:",16);
	OLED_ShowNum(16,32,B6,2,16);		
	OLED_ShowString(60,16,"C:",16);
	OLED_ShowNum(76,16,C6,2,16);
	OLED_ShowString(60,32,"D:",16);		
	OLED_ShowNum(76,32,D6,2,16);
	OLED_Refresh_Gram();
	if(keyvalue==2)
	{
		S6++;
		if(S6>=9)
		{
			S6=9;
		}
	}
	else if(keyvalue==4)
	{
		S6--;
		if(S6==0)
		{
			S6=1;
		}
	}
	else if(keyvalue==3)
	{
		if(I6==0)
		{
			A6=S6;
			I6++;
		}
		else if(I6==1)
		{
			B6=S6;
			I6++;
		}
		else if(I6==2)
		{
			C6=S6;
			I6++;
		}
		else if(I6==3)
		{
			D6=S6;
			I6=0;
		}
	}
	else if(keyvalue==1)
	{
		if(A6==1)
		{
			Ax=75;
			Ay=75;
		}
		else if(A6==2)
		{
			Ax=75;
			Ay=46;
		}
		else if(A6==3)
		{
			Ax=75;
			Ay=19;
		}
		else if(A6==4)
		{
			Ax=47;
			Ay=75;
		}
		else if(A6==5)
		{
			Ax=47;
			Ay=47;
		}
		else if(A6==6)
		{
			Ax=47;
			Ay=19;
		}
		else if(A6==7)
		{
			Ax=19;
			Ay=75;
		}
		else if(A6==8)
		{
			Ax=19;
			Ay=47;
		}
		else if(A6==9)
		{
			Ax=19;
			Ay=19;
		}

		if(B6==1)
		{
			Bx=75;
			By=75;
		}
		else if(B6==2)
		{
			Bx=75;
			By=46;
		}
		else if(B6==3)
		{
			Bx=75;
			By=19;
		}
		else if(B6==4)
		{
			Bx=47;
			By=75;
		}
		else if(B6==5)
		{
			Bx=47;
			By=47;
		}
		else if(B6==6)
		{
			Bx=47;
			By=19;
		}
		else if(B6==7)
		{
			Bx=19;
			By=75;
		}
		else if(B6==8)
		{
			Bx=19;
			By=47;
		}
		else if(B6==9)
		{
			Bx=19;
			By=19;
		}
		if(C6==1)
		{
			Cx=75;
			Cy=75;
		}
		else if(C6==2)
		{
			Cx=75;
			Cy=46;
		}
		else if(C6==3)
		{
			Cx=75;
			Cy=19;
		}
		else if(C6==4)
		{
			Cx=47;
			Cy=75;
		}
		else if(C6==5)
		{
			Cx=47;
			Cy=47;
		}
		else if(C6==6)
		{
			Cx=47;
			Cy=19;
		}
		else if(C6==7)
		{
			Cx=19;
			Cy=75;
		}
		else if(C6==8)
		{
			Cx=19;
			Cy=47;
		}
		else if(C6==9)
		{
			Cx=19;
			Cy=19;
		}
		if(D6==1)
		{
			Dx=75;
			Dy=75;
		}
		else if(D6==2)
		{
			Dx=75;
			Dy=46;
		}
		else if(D6==3)
		{
			Dx=75;
			Dy=19;
		}
		else if(D6==4)
		{
			Dx=47;
			Dy=75;
		}
		else if(D6==5)
		{
			Dx=47;
			Dy=47;
		}
		else if(D6==6)
		{
			Dx=47;
			Dy=19;
		}
		else if(D6==7)
		{
			Dx=19;
			Dy=75;
		}
		else if(D6==8)
		{
			Dx=19;
			Dy=47;
		}
		else if(D6==9)
		{
			Dx=19;
			Dy=19;
		}
	mode6flag=1;
	}
}	
void mode_select(void)
{
	mode=DIP_scan();
	keyvalue=KEY_scan();
	//模式改变则清屏
	if(mode != modelast)
	{
		modelast = mode;
		OLED_Clear();
		
		iii=0;
		tts=0;
	}
	if(mode == 0)
	{
		self_check();
		SetPwm_dj(pwmp0,pwmp1,pwmp2,pwmp3);//复位舵机
	} 
	else if(mode == 1)
	{
		OLED_ShowString(0,0,"BASE1:",16);
		OLED_Refresh_Gram();
	}
	else if(mode == 2)
	{
		OLED_ShowString(0,0,"BASE2:",16);
		OLED_Refresh_Gram();
	}
	else if(mode == 3)
	{
		OLED_ShowString(0,0,"BASE3:",16);
		OLED_Refresh_Gram();
	}	
	else if(mode == 4)
	{
		OLED_ShowString(0,0,"BASE4:",16);
		OLED_Refresh_Gram();
	}	
	else if(mode == 5)
	{
		OLED_ShowString(0,0,"DEVELOP1:",16);
		OLED_Refresh_Gram();
	}
	else if(mode == 6)
	{
		mode6_set();
	}
	else if(mode == 7)
	{
		OLED_ShowString(0,0,"DEVELOP3:",16);
		OLED_Refresh_Gram();
	}
}

void All_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	Stm32_Clock_Init(336,8,2,7); //设置时钟，168Mhz
	uart_init(115200); //串口(蓝牙)初始化，波特率设置115200
	uart2_init(115200);
	uart3_init(115200);
	delay_init(168); //延时初始化
	LED_Init();
	KEY_Init();
	OLED_Init();
	PWM_init_20ms(); //舵机控制pwm初始化
	SetPwm_dj(pwmp0,pwmp1,pwmp2,pwmp3);
	TIM3_Int_Init(1000-1,840-1); //100kHz频率 10ms中断
}

int main(void)
{
	All_init();	
	OLED_Showhzstring(20,20,"滚球控制系统");
	OLED_Refresh_Gram();
	delay_ms(1000);
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
		
		if(mode == 1)
		{		
			//Ctrl_1();
			Ctrl_N1();
		}	
		if(mode == 2)
		{
			//Ctrl_2();
			Ctrl_N2();
		}
		if(mode ==3)
		{
			//Ctrl_3();
			Ctrl_N3();
		}
		if(mode == 4)
		{
			//Ctrl_4();
			Ctrl_N4();
		}
		if(mode == 5)
		{
			//Ctrl_5();
			Ctrl_N5();
		}
		if(mode == 6&&mode6flag==1)
		{
			Ctrl_N6();
		}
		if(mode == 7)
		{
			//Ctrl_7();
			Ctrl_N7();
		}
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


void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		x_now=USART_ReceiveData(USART2);	//读取接收到的数据
	}
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		y_now=USART_ReceiveData(USART3);
	}
}
