#include "csb.h"
#include "pos_mach.h"
#include "FILTER.h"

/*初始化模块的GPIO以及初始化定时器TIM2*/
void CH_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/*定时器TIM2初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);  ///使能TIM2时钟
	

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=168-1;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
	
	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置


	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	PBout(8)=1;  //拉高信号，作为触发信号
	delay_us(20);  //高电平信号超过10us
	PBout(8)=0;
}

u8 aa=0;
float length=0;
float length_old = 0;
float length_new = 0;

void EXTI9_5_IRQHandler(void)
{
	u16 tim;
	aa++;
	if(aa == 1)
	{
		TIM2->CNT=0;
	}
	else
	{
		tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间
		length_new = tim*0.034f;//通过回响信号计算距离	
		if(length_new >= 90)
		{
			length_new = length_old;
		}
		length = Kalman_Filter(length_new,length_new - length_old);
		OLED_ShowNum(50,32,length,5,16);
		mpu6050_send_data(length,length_new,0,0,0,0);
		length_old = length_new;
		PBout(8)=1;  //拉高信号，作为触发信号
		delay_us(20);  //高电平信号超过10us
		PBout(8)=0;
		TIM2->CNT=0;
		aa=0;	
	}
	EXTI_ClearITPendingBit(EXTI_Line9);//清除LINE2上的中断标志位 
}
