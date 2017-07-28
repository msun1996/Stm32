#include "key.h"
#include "delay.h" 

//按键初始化函数
void KEY_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD
 
} 

u8 DIP_scan(void)
{
	if(DIP1==0 && DIP2==0 && DIP3==0)return 0;
	else if(DIP1==1 && DIP2==0 && DIP3==0)return 1;
	else if(DIP1==0 && DIP2==1 && DIP3==0)return 2;
	else if(DIP1==1 && DIP2==1 && DIP3==0)return 3;
	else if(DIP1==0 && DIP2==0 && DIP3==1)return 4;
	else if(DIP1==1 && DIP2==0 && DIP3==1)return 5;
	else if(DIP1==0 && DIP2==1 && DIP3==1)return 6;
	else if(DIP1==1 && DIP2==1 && DIP3==1)return 7;
	else return 8;
}

u8 KEY_scan(void)
{
	static u8 key_up=1; //按键松开标志
	if(key_up && (KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1;
	return 0;
}
