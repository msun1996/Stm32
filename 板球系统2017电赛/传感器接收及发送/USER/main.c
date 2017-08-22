#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

//ALIENTEK ̽����STM32F407������ ʵ��0
//STM32F4����ģ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

extern u16 xxx;
u16 C_Pin,D_Pin,E_Pin;
u16 yyy;
uint16_t Pin[16] = {0x0001,0x0002,0x0004,0x008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};

void GPIO_All_Init(void);
void GPIO_Scan(void);

int main(void)
{
	uart_init(115200);
	delay_init(84);
	GPIO_All_Init();
	TIM3_Int_Init(100-1,840-1); //100kHzƵ�� 1ms�ж�
	while(1);
}


void GPIO_All_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOB all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOC all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOD all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOE all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PE
}

void GPIO_Scan(void)
{
	u8 count=0; //ͳ�Ʋ�����
	u8 i;
	
	C_Pin = GPIO_ReadInputData(GPIOC);
	D_Pin = GPIO_ReadInputData(GPIOD);
	E_Pin = GPIO_ReadInputData(GPIOE);
	
	yyy = 0; //��ʼ��0
	
	if(C_Pin != 0x0000)
	{
		for(i=0; i<=15; i++)
		{
			if(GPIO_ReadInputDataBit(GPIOC,Pin[i]) == 1)
			{
				count++;
				yyy = 1+i+yyy;
			}
		}
	}
	if(D_Pin != 0x0000)
	{
		for(i=0; i<=15; i++)
		{
			if(GPIO_ReadInputDataBit(GPIOD,Pin[i]) == 1)
			{
				count++;
				yyy = 17 + i + yyy; 
			}
		}
	}
    if(E_Pin != 0x0000)
	{
		for(i=0; i<=15; i++)
		{
			if(GPIO_ReadInputDataBit(GPIOE,Pin[i]) == 1)
			{
				count++;
				yyy = 33 + i + yyy; 
			}
		}
	}
	
	yyy = yyy * 2 / count; //�������е�����ֵ
	
	usart1_send_char(yyy);
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{	
		GPIO_Scan();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
