#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

//ALIENTEK 探索者STM32F407开发板 实验0
//STM32F4工程模板-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

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
	TIM3_Int_Init(100-1,840-1); //100kHz频率 1ms中断
	while(1);
}


void GPIO_All_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOB all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOC all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOD all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //GPIOE all pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PE
}

void GPIO_Scan(void)
{
	u8 count=0; //统计测量点
	u8 i;
	
	C_Pin = GPIO_ReadInputData(GPIOC);
	D_Pin = GPIO_ReadInputData(GPIOD);
	E_Pin = GPIO_ReadInputData(GPIOE);
	
	yyy = 0; //初始设0
	
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
	
	yyy = yyy * 2 / count; //计算所有挡点中值
	
	usart1_send_char(yyy);
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{	
		GPIO_Scan();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
