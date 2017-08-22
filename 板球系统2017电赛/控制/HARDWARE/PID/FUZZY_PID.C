#include "FUZZY_PID.h"

char  A_E[7] = {1,2,3,4,5,6,7};  //输入E模糊集  分别为: NB,NM,NS,ZO,PS,PM,PB
char B_EC[7] = {1,2,3,4,5,6,7};	 //输入EC模糊集
char    U[7] = {1,2,3,4,5,6,7};  //输出U模糊集
	
char Rule_P[7][7] = {{7,7,6,6,5,4,4},
										 {7,7,6,5,5,4,3},
										 {6,6,6,5,4,3,3},
										 {6,6,5,4,3,2,2},
										 {5,5,4,3,3,2,2},
										 {5,4,3,2,2,2,1},
										 {4,4,2,2,2,1,1}};

char Rule_I[7][7] = {{1,1,2,2,3,4,4},
										 {1,1,2,3,3,4,4},
										 {1,2,3,3,4,5,5},
										 {2,2,3,4,5,6,6},
										 {2,3,4,5,5,6,7},
										 {4,4,5,5,6,7,7},
										 {4,4,5,6,6,7,7}};

char Rule_D[7][7] = {{5,3,1,1,1,2,5},
										 {5,3,1,2,2,3,4},
										 {4,3,2,2,3,3,4},
										 {4,3,3,3,3,3,4},
										 {4,4,4,4,4,4,4},
										 {7,3,5,5,5,5,7},
										 {7,6,6,6,5,5,7}};
	
char   E_mohu[2];    //输入E模糊值
float E_lishu[2];    //输入E隶属度 

char   EC_mohu[2];	 //输入EC模糊值
float EC_lishu[2];   //输入EC隶属度 

char   U_mohu_P[4];	 //输出U_P模糊值
float U_lishu_P[4];	 //输出U_P隶属度 
										 
char   U_mohu_I[4];	 //输出U_I模糊值
float U_lishu_I[4];	 //输出U隶属度 
										 
char   U_mohu_D[4];	 //输出U_I模糊值
float U_lishu_D[4];	 //输出U_D隶属度 

float max(float m,float n)
{
	if(m>n) return m;
	else return n;
}

float min(float m,float n)
{
	if(m>n) return n;
	else return m;
}

//三角形隶属函数
//x:输入变量 
//[a,c]:函数定义域区间  
//b:函数定义域中点
float Triangle(float x,float a,float b,float c)   
{
	if(x<b&&x>=a) return (x-a)/(b-a);
	else if(x<c&&x>=b)   return (c-x)/(c-b);
	else 	
		return 0;
}

//输入量化为论域
//data: 量化数据
//opt: 选项
//  opt_e: 量化E
// opt_ec: 量化EC
float lianghua(float data,u8 opt)
{
	if(opt==opt_e)
		return (K_E*(data - (E_MAX + E_MIN)/2));
	else if(opt==opt_ec)
		return (K_EC*(data - (EC_MAX + EC_MIN)/2));	
	else 
		return 0;
}

//输出还原为真实值
//data: 还原数据
// opt: 选项
// opt_p: 选择还原输出P
// opt_i: 选择还原输出i
// opt_d: 选择还原输出d
float huanyuan(float data,char opt)
{
	float k,max,min;
	if(opt==opt_p)	
	{
		  k = K_U_P;
		max = U_MAX_P;
		min = U_MIN_P;
	}
	else if(opt==opt_i)  
	{
		  k = K_U_I;
		max = U_MAX_I;
		min = U_MIN_I;
	}	else if(opt==opt_d)  
	{
		  k = K_U_D;
		max = U_MAX_D;
		min = U_MIN_D;
	}
	return k*data + (max + min)/2;
}

//三角函数隶属度计算并输出对应模糊值
//x: 量化数据输入，取值须在[-6,6]内
//opt:  选项
// opt_e: 计算E的隶属度和对应模糊值
//opt_ec: 计算EC的隶属度和对应模糊值
//  函数定义域区间  
void mohuzhi_lishudu(float x,char opt)		
{	
	if(opt==opt_e)
	{
		if(x < -6)
		{
			 E_mohu[0] = 1;
			E_lishu[0] = 1;				
			 E_mohu[1] = 2;
			E_lishu[1] = 0;	
		}	
		else if(x >= -6&&x < -4)
		{
			 E_mohu[0] = 1;
			E_lishu[0] = Triangle(x,-6,-6,-4);
			 E_mohu[1] = 2;
			E_lishu[1] = Triangle(x,-6,-4,-2);			
		}
		else if(x >= -4&&x < -2)
		{
			 E_mohu[0] = 2;
			E_lishu[0] = Triangle(x,-6,-4,-2);
			 E_mohu[1] = 3;
			E_lishu[1] = Triangle(x,-4,-2,0);				
		}
		else if(x >= -2&&x < 0)
		{
			 E_mohu[0] = 3;
			E_lishu[0] = Triangle(x,-4,-2,0);				
			 E_mohu[1] = 4;
			E_lishu[1] = Triangle(x,-2,0,2);				
		}
		else if(x >= 0&&x < 2)
		{
			 E_mohu[0] = 4;
			E_lishu[0] = Triangle(x,-2,0,2);				
			 E_mohu[1] = 5;
			E_lishu[1] = Triangle(x,0,2,4);				
		}
		else if(x >= 2&&x < 4)
		{
			 E_mohu[0] = 5;
			E_lishu[0] = Triangle(x,0,2,4);				
			 E_mohu[1] = 6;
			E_lishu[1] = Triangle(x,2,4,6);				
		}
		else if(x >= 4&&x < 6)
		{
			 E_mohu[0] = 6;
			E_lishu[0] = Triangle(x,2,4,6);				
			 E_mohu[1] = 7;
			E_lishu[1] = Triangle(x,4,6,6);				
		}
		else 
		{
		 	 E_mohu[0] = 6;
			E_lishu[0] = 0;				
		   E_mohu[1] = 7;
			E_lishu[1] = 1;							
		}
	}
	else if(opt==opt_ec)
	{
		if(x < -6)
		{
			 EC_mohu[0] = 1;
			EC_lishu[0] = 1;				
			 EC_mohu[1] = 2;
			EC_lishu[1] = 0;	
		}	
		else if(x >= -6&&x < -4)
		{
			 EC_mohu[0] = 1;
			EC_lishu[0] = Triangle(x,-6,-6,-4);
			 EC_mohu[1] = 2;
			EC_lishu[1] = Triangle(x,-6,-4,-2);			
		}
		else if(x >= -4&&x < -2)
		{
			 EC_mohu[0] = 2;
			EC_lishu[0] = Triangle(x,-6,-4,-2);
			 EC_mohu[1] = 3;
			EC_lishu[1] = Triangle(x,-4,-2,0);				
		}
		else if(x >= -2&&x < 0)
		{
			 EC_mohu[0] = 3;
			EC_lishu[0] = Triangle(x,-4,-2,0);				
			 EC_mohu[1] = 4;
			EC_lishu[1] = Triangle(x,-2,0,2);				
		}
		else if(x >= 0&&x < 2)
		{
			 EC_mohu[0] = 4;
			EC_lishu[0] = Triangle(x,-2,0,2);				
			 EC_mohu[1] = 5;
			EC_lishu[1] = Triangle(x,0,2,4);				
		}
		else if(x >= 2&&x < 4)
		{
			 EC_mohu[0] = 5;
			EC_lishu[0] = Triangle(x,0,2,4);				
			 EC_mohu[1] = 6;
			EC_lishu[1] = Triangle(x,2,4,6);				
		}
		else if(x >= 4&&x < 6)
		{
		   EC_mohu[0] = 6;
			EC_lishu[0] = Triangle(x,2,4,6);				
			 EC_mohu[1] = 7;
			EC_lishu[1] = Triangle(x,4,6,6);				
		}
		else 
		{
			 EC_mohu[0] = 6;
			EC_lishu[0] = 0;				
			 EC_mohu[1] = 7;
			EC_lishu[1] = 1;							
		}			
	}
}

//模糊规则查表
//   *e_mohu: 为输入偏差E的模糊值首地址
//  *e_lishu: 为输入偏差E的隶属度首地址
//  *ec_mohu: 为输入偏差EC的模糊值首地址
// *ec_lishu: 为输入偏差EC的隶属度首地址
// opt: 选项
//opt_p：查表求P的模糊值及隶属度
//opt_i：查表求I的模糊值及隶属度
//opt_d：查表求D的模糊值及隶属度
void Rule_Check(char *e_mohu,float *e_lishu,char *ec_mohu,float *ec_lishu,char opt)
{
	if(opt==opt_p)
	{
		 U_mohu_P[0] = Rule_P[e_mohu[0] - 1][ec_mohu[0] - 1];			
		U_lishu_P[0] = min(e_lishu[0],ec_lishu[0]);			

		 U_mohu_P[1] = Rule_P[e_mohu[0] - 1][ec_mohu[1] - 1];			
		U_lishu_P[1] = min(e_lishu[0],ec_lishu[1]);			

		 U_mohu_P[2] = Rule_P[e_mohu[1] - 1][ec_mohu[0] - 1];			
		U_lishu_P[2] = min(e_lishu[1],ec_lishu[0]);			

		 U_mohu_P[3] = Rule_P[e_mohu[1] - 1][ec_mohu[1] - 1];			
		U_lishu_P[3] = min(e_lishu[1],ec_lishu[1]);			
	}
	else if(opt==opt_i)
	{
		 U_mohu_I[0] = Rule_I[e_mohu[0] - 1][ec_mohu[0] - 1];			
		U_lishu_I[0] = min(e_lishu[0],ec_lishu[0]);			

		 U_mohu_I[1] = Rule_I[e_mohu[0] - 1][ec_mohu[1] - 1];			
		U_lishu_I[1] = min(e_lishu[0],ec_lishu[1]);			

		 U_mohu_I[2] = Rule_I[e_mohu[1] - 1][ec_mohu[0] - 1];			
		U_lishu_I[2] = min(e_lishu[1],ec_lishu[0]);			

		 U_mohu_I[3] = Rule_I[e_mohu[1] - 1][ec_mohu[1] - 1];			
		U_lishu_I[3] = min(e_lishu[1],ec_lishu[1]);			
	}
	else if(opt==opt_d)
	{
		 U_mohu_D[0] = Rule_D[e_mohu[0] - 1][ec_mohu[0] - 1];			
		U_lishu_D[0] = min(e_lishu[0],ec_lishu[0]);			

		 U_mohu_D[1] = Rule_D[e_mohu[0] - 1][ec_mohu[1] - 1];			
		U_lishu_D[1] = min(e_lishu[0],ec_lishu[1]);			

		 U_mohu_D[2] = Rule_D[e_mohu[1] - 1][ec_mohu[0] - 1];			
		U_lishu_D[2] = min(e_lishu[1],ec_lishu[0]);			

		 U_mohu_D[3] = Rule_D[e_mohu[1] - 1][ec_mohu[1] - 1];			
		U_lishu_D[3] = min(e_lishu[1],ec_lishu[1]);			
	}
}
//重心法解模糊
// *u_mohu: 为输出模糊值
//*u_lishu: 为输出隶属度
//重心法解模糊计算公式: 求和(lishudu[i]*c[i])/求和(lishudu[i])  c[i]为模糊值为i的三角型隶属函数的中心元素
float jiemohu(char *u_mohu,float *u_lishu)
{
	char i;
	float fenzi;
	float fenmu;
	for(i=0;i<16;i++)
	{
		fenmu += u_lishu[i];
    fenzi += u_lishu[i]*(2*(float)u_mohu[i] - 8);
	}
	 return fenzi/fenmu;
}


mohu_out fuzzy(float e,float ec)
{
	float E_temp,EC_temp;
	mohu_out temp;
	
	E_temp  = lianghua(e,opt_e);
	EC_temp = lianghua(ec,opt_ec);
	
	mohuzhi_lishudu(E_temp,opt_e);	
	mohuzhi_lishudu(EC_temp,opt_ec);	
	
	Rule_Check(&E_mohu[0],&E_lishu[0],&EC_mohu[0],&EC_lishu[0],opt_p);
	Rule_Check(&E_mohu[0],&E_lishu[0],&EC_mohu[0],&EC_lishu[0],opt_i);
	Rule_Check(&E_mohu[0],&E_lishu[0],&EC_mohu[0],&EC_lishu[0],opt_d);

	temp.p = jiemohu(&U_mohu_P[0],&U_lishu_P[0]);
	temp.i = jiemohu(&U_mohu_I[0],&U_lishu_I[0]);
	temp.d = jiemohu(&U_mohu_D[0],&U_lishu_D[0]);
	
	temp.p = huanyuan(temp.p,opt_p);
	temp.i = huanyuan(temp.i,opt_i);
	temp.d = huanyuan(temp.d,opt_d);

	return 	temp;
}








