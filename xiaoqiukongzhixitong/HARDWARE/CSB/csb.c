#include "csb.h"
#include "pos_mach.h"
#include "FILTER.h"

/*��ʼ��ģ���GPIO�Լ���ʼ����ʱ��TIM2*/
void CH_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/*��ʱ��TIM2��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);  ///ʹ��TIM2ʱ��
	

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=168-1;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
	
	/* ����EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����


	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	PBout(8)=1;  //�����źţ���Ϊ�����ź�
	delay_us(20);  //�ߵ�ƽ�źų���10us
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
		tim=TIM_GetCounter(TIM2);//��ȡ��TIM2���Ĵ����еļ���ֵ��һ�߼�������ź�ʱ��
		length_new = tim*0.034f;//ͨ�������źż������	
		if(length_new >= 90)
		{
			length_new = length_old;
		}
		length = Kalman_Filter(length_new,length_new - length_old);
		OLED_ShowNum(50,32,length,5,16);
		mpu6050_send_data(length,length_new,0,0,0,0);
		length_old = length_new;
		PBout(8)=1;  //�����źţ���Ϊ�����ź�
		delay_us(20);  //�ߵ�ƽ�źų���10us
		PBout(8)=0;
		TIM2->CNT=0;
		aa=0;	
	}
	EXTI_ClearITPendingBit(EXTI_Line9);//���LINE2�ϵ��жϱ�־λ 
}
