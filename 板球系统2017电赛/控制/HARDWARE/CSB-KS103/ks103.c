#include "ks103.h"
#include "delay.h"
#include "iic.h"

void KS103_Init(void)
{
	IIC_Init();
	KS103_WriteOneByte(0XE8,0X02,0X73); //消噪配置
	delay_ms(2000);
}


u8 KS103_ReadOneByte(u8 address, u8 reg)
{ 
u8 temp=0;
IIC_Start();
IIC_Send_Byte(address); //发送低地址
IIC_Wait_Ack();
IIC_Send_Byte(reg); //发送低地址
IIC_Wait_Ack();
IIC_Start();
IIC_Send_Byte(address + 1); //进入接收模式 
IIC_Wait_Ack();
delay_us(50);  //增加此代码通信成功！！！
temp=IIC_Read_Byte(0);  //读寄存器 3
IIC_Stop();//产生一个停止条件 
return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{ 
IIC_Start();
IIC_Send_Byte(address);  //发送写命令
IIC_Wait_Ack();
IIC_Send_Byte(reg);//发送高地址
IIC_Wait_Ack();
IIC_Send_Byte(command); //发送低地址
IIC_Wait_Ack();
IIC_Stop();//产生一个停止条件
}

void KS103_Send(void) //发送测距信号 
{
	KS103_WriteOneByte(0XE8,0X02,0Xb0);
}

u16 KS103_Get_Date(void) //执行读取前应该先执行发送指令 时间差按数据数册  50ms左右 
{
	u16 dist;
	dist = KS103_ReadOneByte(0xe8, 0x02);
	dist <<= 8;
	dist += KS103_ReadOneByte(0xe8, 0x03);
	return dist;
}
