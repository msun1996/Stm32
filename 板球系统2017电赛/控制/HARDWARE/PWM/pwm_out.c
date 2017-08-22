/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
 * 文件名  ：pwm_out.c
 * 描述    ：PWM输出
 *
 *			pwm_out1 PE14  6
 *			pwm_out2 PE13  8
 *			pwm_out3 PE11  7
 *			pwm_out4 PE9   5
 *			pwm_out5 PA3   3
 *			pwm_out6 PA2   1
 *			pwm_out7 PC9   4
 *			pwm_out8 PC8   2
**********************************************************************************/

#include "pwm_out.h"

#define INIT_DUTY 	5000 
#define ACCURACY 	10000

//Run TIM_SetAutoreload(),TIM_SetCompare1() function to change the frequent and duty;
//84000000/21 = 4M   1/4M = 0.25us
//Interrurt Time = 1/(84MHZ /21) * 10000 = 2.5ms

u8 CH_out_Mapping[8] = {0,1,2,3,4,5,6,7};			//8 means have 8 motor output

u8 PWM_Out_Init(uint16_t hz)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t PrescalerValue = 0;
	u32 hz_set = ACCURACY*hz;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);

	/////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( ( SystemCoreClock /2 ) / hz_set ) - 1;  //hz*10000(Hz)
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
	
	
	/////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( ( SystemCoreClock ) / hz_set ) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);	
	
	
	////////////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ( ( SystemCoreClock ) / hz_set ) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = ACCURACY;									
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);


	if( hz_set > 84000000 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void SetPwm(u16 pwm0,u16 pwm1,u16 pwm2,u16 pwm3,u16 pwm4,u16 pwm5,u16 pwm6,u16 pwm7)
{

	TIM1->CCR4 = pwm5;			
	TIM1->CCR3 = pwm7;
	TIM1->CCR2 = pwm6;
	TIM1->CCR1 = pwm4;
	
 	TIM5->CCR4 = pwm2;
 	TIM5->CCR3 = pwm0;
 	TIM8->CCR4 = pwm3;
	TIM8->CCR3 = pwm1;
}


//步进电机控制

//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void Step_PWM_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 100;    //自动重装载值							
	TIM_TimeBaseStructure.TIM_Prescaler = 84;	 //定时器分频 1us计数一次	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	//PWM初始化
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性:TIM输出比较极性高

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //初始占空比
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE); //ARPE使能 
	TIM_Cmd(TIM5, ENABLE); //使能TIM5
	
}

void step_PWM(u32 num)
{
	u32 arr = 10000/num;
	if(arr == 0)
	{
		TIM5->ARR = 1000;
		TIM5->CCR4 = 0;
	}
	else
	{
		TIM5->ARR = arr;
		TIM5->CCR4 = arr/2;
	}	
}


//舵机控制
//设置周期20ms

void PWM_init_20ms(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 20000; //自动重装值 20000*1us									
	TIM_TimeBaseStructure.TIM_Prescaler = 168;		
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);	
}

void SetPwm_dj(u16 pwm4,u16 pwm5,u16 pwm6,u16 pwm7)
{
	if(pwm4<=600)
	{
		pwm4=600;
	}
	else if(pwm4 >=2400)
	{
		pwm4=2400;
	}
	if(pwm5<=600)
	{
		pwm5=600;
	}
	else if(pwm5 >=2400)
	{
		pwm5=2400;
	}
	if(pwm6<=600)
	{
		pwm6=600;
	}
	else if(pwm6 >=2400)
	{
		pwm6=2400;
	}
	if(pwm7<=600)
	{
		pwm7=600;
	}
	else if(pwm7 >=2400)
	{
		pwm7=2400;
	}
	TIM1->CCR4 = pwm5;
	TIM1->CCR3 = pwm7;
	TIM1->CCR2 = pwm6;
	TIM1->CCR1 = pwm4;
}
