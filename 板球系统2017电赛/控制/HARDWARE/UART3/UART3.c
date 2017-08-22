#include "UART3.h"

void uart3_init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//����3����
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	/* Configure USARTx_Tx as alternate function push-pull */

	//uart3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
}


void usart3_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
	USART_SendData(USART3,c);
}

void usart3_niming_report(u8 *data, u8 len)
{
	u8 send3_buf[16];
	u8 i;
	send3_buf[len+2]=0;//У������0
	send3_buf[0]=0xAA; //֡ͷ
	send3_buf[1]=len;  //���ݳ���
	for(i=0;i<len;i++)send3_buf[2+i]=data[i];
	for(i=0;i<len+2;i++)send3_buf[len+2]+=send3_buf[i]; //����У���
	for(i=0;i<len+3;i++)usart3_send_char(send3_buf[i]); //���ʹ���3
}
void usart3_send_data(short data1, short data2, short data3)
{
	u8 tbuf3[10];
	tbuf3[0]=(data1>>8)&0XFF;
	tbuf3[1]=data1&0XFF;
	tbuf3[2]=(data2>>8)&0XFF;
	tbuf3[3]=data2&0XFF;
	tbuf3[4]=(data3>>8)&0XFF;
	tbuf3[5]=data3&0XFF;
	tbuf3[6]=0;
	tbuf3[7]=0;
	tbuf3[8]=0;
	tbuf3[9]=0;
	usart3_niming_report(tbuf3,10);
}

//Ԥ����
void usart3_receive_char(u8 data)
{
	static u8 RxBuffer3[50];
	static u8 _data_len3 = 0,_data_cnt3 = 0;
	static u8 state3 = 0;
	if(state3==0&&data==0xAA)
	{
		state3=1;
		RxBuffer3[0]=data;//֡ͷ
	}
	else if(state3==1)
	{
		state3=2;
		RxBuffer3[1]=data;//����
		_data_len3 = data;
	}
	else if(state3==2&&_data_len3>0)
	{
		_data_len3--;
		RxBuffer3[2+_data_cnt3++]=data;
		if(_data_len3 == 0)
			state3=3;
	}
	else if(state3 == 3) //���ݽ���
	{
		state3 = 0;
		RxBuffer3[2+_data_cnt3]=data;
		Data_Receive_Anl3(RxBuffer3,_data_cnt3+3);
	}
	else
		state3 = 0;
}

//���ݽ���
void Data_Receive_Anl3(u8 *data_buf,u8 num)
{
	u8 sum=0;
	u8 i=0;
	for(i=0;i<(num-1);i++)
		sum +=*(data_buf+i);
	if(!(sum==*(data_buf+num-1)))return;//�ж�sum
	if(!(*(data_buf)==0xAA))return;//�ж�֡ͷ
	//x_now = (vs16)(*(data_buf+2)<<8)|*(data_buf+3);
	//y_now = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
	//zz = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
	
}
