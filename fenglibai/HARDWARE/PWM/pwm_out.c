/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
 * 文件名  ：pwm_out.c
 * 描述    ：PWM输出
 *
 *			pwm_out1 PE14
 *			pwm_out2 PE13 
 *			pwm_out3 PE11
 *			pwm_out4 PE9
 *			pwm_out5 PA3
 *			pwm_out6 PA2
 *			pwm_out7 PC9
 *			pwm_out8 PC8
**********************************************************************************/

#include "pwm_out.h"

#define INIT_DUTY 	0 	
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

	TIM1->CCR4 = pwm0;			
	TIM1->CCR3 = pwm1;
	TIM1->CCR2 = pwm2;
	TIM1->CCR1 = pwm3;
	
 	TIM5->CCR4 = pwm4;
 	TIM5->CCR3 = pwm5;
 	TIM8->CCR4 = pwm6;
	TIM8->CCR3 = pwm7;
	
}

