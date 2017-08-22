#include "ks103.h"
#include "delay.h"
#include "iic.h"

void KS103_Init(void)
{
	IIC_Init();
	KS103_WriteOneByte(0XE8,0X02,0X73); //��������
	delay_ms(2000);
}


u8 KS103_ReadOneByte(u8 address, u8 reg)
{ 
u8 temp=0;
IIC_Start();
IIC_Send_Byte(address); //���͵͵�ַ
IIC_Wait_Ack();
IIC_Send_Byte(reg); //���͵͵�ַ
IIC_Wait_Ack();
IIC_Start();
IIC_Send_Byte(address + 1); //�������ģʽ 
IIC_Wait_Ack();
delay_us(50);  //���Ӵ˴���ͨ�ųɹ�������
temp=IIC_Read_Byte(0);  //���Ĵ��� 3
IIC_Stop();//����һ��ֹͣ���� 
return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{ 
IIC_Start();
IIC_Send_Byte(address);  //����д����
IIC_Wait_Ack();
IIC_Send_Byte(reg);//���͸ߵ�ַ
IIC_Wait_Ack();
IIC_Send_Byte(command); //���͵͵�ַ
IIC_Wait_Ack();
IIC_Stop();//����һ��ֹͣ����
}

void KS103_Send(void) //���Ͳ���ź� 
{
	KS103_WriteOneByte(0XE8,0X02,0Xb0);
}

u16 KS103_Get_Date(void) //ִ�ж�ȡǰӦ����ִ�з���ָ�� ʱ����������  50ms���� 
{
	u16 dist;
	dist = KS103_ReadOneByte(0xe8, 0x02);
	dist <<= 8;
	dist += KS103_ReadOneByte(0xe8, 0x03);
	return dist;
}
