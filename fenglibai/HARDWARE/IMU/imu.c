#include "imu.h"
#include "math.h"
#include "stm32f4xx.h"
#include "mpu6050.h"


//一阶滤波算法
#define K1 0.1 //对加速度机
#define 
/*
//四元数滤波
#define Kp 100.0f //比例增益支配率收敛到加速度计
#define Ki 0.002f  //积分增益支配率的陀螺仪偏见的衔接
#define halfT 0.001f  // 采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; //四元数的元素，代表估计方向
float exInt = 0, eyInt = 0, ezInt = 0; // 按比例积分误差

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az,float *pitch,float *roll)
{

        float norm;
        float vx, vy, vz;
        float ex, ey, ez;  
        norm = sqrt(ax*ax + ay*ay + az*az);      

        ax = ax / norm;  
        ay = ay / norm;
        az = az / norm;      

        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

        ex = (ay*vz - az*vy);
        ey = (az*vx - ax*vz);
        ez = (ax*vy - ay*vx);


        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;

        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;

        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

        q0 = q0 / norm;
        q1 = q1 / norm;
        q2 = q2 / norm;
        q3 = q3 / norm;

        *pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 
        *roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 

}
*/