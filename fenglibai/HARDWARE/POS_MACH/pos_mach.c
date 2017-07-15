#include "pos_mach.h"

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
	USART_SendData(USART1,c);  
}
//�������ݸ�����������λ�����
//fun:������. 0X00~0XFF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+4]=0;	//У��������
	send_buf[0]=0XAA;	//֡ͷ
	send_buf[1]=0xAA;
	send_buf[2]=fun;	//������
	send_buf[3]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//��������
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//����У���	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[18]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	tbuf[12]=0;
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	tbuf[16]=0;
	tbuf[17]=0;
	usart1_niming_report(0x02,tbuf,18);
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short roll,short pitch,short yaw)
{
	u8 tbuf[12]; 
	u8 i;
	for(i=0;i<12;i++)tbuf[i]=0;//��0
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF; 
	tbuf[6]=0;
	tbuf[7]=0;
	tbuf[8]=0;
	tbuf[9]=0;
	tbuf[10]=0;
	tbuf[11]=1;	
	usart1_niming_report(0X01,tbuf,12);
}
//����PWM����λ��
void Data_send_MotoPWM(short MotoPWM1,short MotoPWM2,short MotoPWM3,short MotoPWM4)
{
	u8 tbuf[16];
	tbuf[0]=(MotoPWM1>>8)&0xff;
	tbuf[1]=MotoPWM1&0xff;
	tbuf[2]=(MotoPWM2>>8)&0xff;
	tbuf[3]=MotoPWM2&0xff;
	tbuf[4]=(MotoPWM3>>8)&0xff;
	tbuf[5]=MotoPWM3&0xff;
	tbuf[6]=(MotoPWM4>>8)&0xff;
	tbuf[7]=MotoPWM4&0xff;
	tbuf[8]=0;
	tbuf[9]=0;
	tbuf[10]=0;
	tbuf[11]=0;
	tbuf[12]=0;
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	usart1_niming_report(0X06,tbuf,16);

}

//����У��λ
void send_receive_check(u8 head,u8 check_sum)
{
	u8 tbuf[16];
	u8 sum = 0;
	u8 i=0;
	
	tbuf[0]=head;
	tbuf[1]=check_sum;
	for(i=0;i<6;i++)
		sum += tbuf[i];
	tbuf[2]=sum;
	usart1_niming_report(0Xef,tbuf,7);
}

//��Ƭ��Ԥ������λ��
void usart1_receive_char(u8 data)
{   	
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;//֡ͷ
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;//������
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;//����
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)//�������ݽ���
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}
//���ݽ���
void Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	u8 i=0;
	for(i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	if(*(data_buf+2)==0X01)//������У��
	{
		if(*(data_buf+4)==0X01)
			//mpu6050.Acc_CALIBRATE = 1;
		if(*(data_buf+4)==0X02)
			//mpu6050.Gyro_CALIBRATE = 1;
		if(*(data_buf+4)==0X03)
		{
			//mpu6050.Acc_CALIBRATE = 1;		
			//mpu6050.Gyro_CALIBRATE = 1;			
		}
	}
	if(*(data_buf+2)==0X10)								//PID1_3  
  {
        //fly_pitch_Kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        //fly_pitch_Ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        //fly_pitch_Kd = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        //fly_roll_Kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        //fly_roll_Ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        //fly_roll_Kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        //fly_yaw_Kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        //fly_yaw_Ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        //fly_yaw_Kd = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
			send_receive_check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X11)								//PID4_6
  {
        //ctrl_1.PID[PID4].kp   =0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        //ctrl_1.PID[PID4].ki   =0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        //ctrl_1.PID[PID4].kd   =0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        //ctrl_1.PID[PID5].kp 	= 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        //ctrl_1.PID[PID5].ki 	= 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        //ctrl_1.PID[PID5].kd 	= 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        //ctrl_1.PID[PID6].kp	  = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        //ctrl_1.PID[PID6].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        //ctrl_1.PID[PID6].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
		    send_receive_check(*(data_buf+2),sum);
	}
  if(*(data_buf+2)==0X12)								//PID7_9
  {	
        //fly_gas=(vs16)(*(data_buf+4)<<8)|*(data_buf+5);
        //ctrl_2.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        //ctrl_2.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        //ctrl_2.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        //ctrl_2.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        //ctrl_2.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        //ctrl_2.PID[PIDYAW].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        //ctrl_2.PID[PIDYAW].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        //ctrl_2.PID[PIDYAW].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
			send_receive_check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X13)								//PID10_12
	{
		     send_receive_check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID13_15
	{
		     send_receive_check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID16_18
	{
		     send_receive_check(*(data_buf+2),sum);
	}

}


