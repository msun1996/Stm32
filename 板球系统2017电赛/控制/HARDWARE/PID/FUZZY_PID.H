#ifndef __FUZZY_PID_H
#define __FUZZY_PID_H

#include "sys.h"

#define E_MAX  100
#define E_MIN  0

#define EC_MAX  100
#define EC_MIN  0

#define U_MAX_P  10.0f
#define U_MIN_P  -10.0f
#define U_MAX_I  0.0000001f
#define U_MIN_I  -0.0000001f
#define U_MAX_D  100.0f
#define U_MIN_D  -100.0f

#define N_E_EC  6                            //输入论域半长[-6,6]

#define N_U_P     6                            //输出论域KP半长[-3,3]
#define N_U_I     6                            //输出论域KI半长[-3,3]
#define N_U_D     6                            //输出论域KD半长[-3,3]

#define K_E     (float)2*N_E_EC/(E_MAX - E_MIN)     //E量化因子
#define K_EC    (float)2*N_E_EC/(EC_MAX - EC_MIN)   //EC量化因子

#define K_U_P     (float)(U_MAX_P - U_MIN_P)/(2*N_U_P)      //U_P比例因子
#define K_U_I     (float)(U_MAX_I - U_MIN_I)/(2*N_U_I)      //U_I比例因子
#define K_U_D     (float)(U_MAX_D - U_MIN_D)/(2*N_U_D)      //U_D比例因子

#define opt_p   0      //模糊控制选项
#define opt_i   1    
#define opt_d   2

#define opt_e   0      //模糊控制选项
#define opt_ec  1

typedef struct         //定义模糊控制输出结构体
{
	float p;
	float i;
	float d;
}mohu_out;

float max(float m,float n);
float min(float m,float n);
float Triangle(float x,float a,float b,float c); 
float lianghua(float data,u8 opt);
float huanyuan(float data,char opt);
void mohuzhi_lishudu(float x,char opt);		
void Rule_Check(char *e_mohu,float *e_lishu,char *ec_mohu,float *ec_lishu,char opt);
float jiemohu(char *u_mohu,float *u_lishu);

mohu_out fuzzy(float e,float ec);

#endif

