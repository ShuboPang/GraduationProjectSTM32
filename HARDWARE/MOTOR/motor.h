#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//motor驱动代码	   
//STM32F4工程-库函数版本								  
////////////////////////////////////////////////////////////////////////////////// 	

#define MOTOR_NUM 3
#define MOTOR_START_NUM 1

//电机端口定义
#define MOTOR1EN PEout(7)	
#define MOTOR1DIR PEout(8)	
#define MOTOR1 PEout(9)	 
#define MOTOR2EN PEout(10)	
#define MOTOR2DIR PEout(11)	
#define MOTOR2 PEout(12)
#define MOTOR3EN PEout(13)	
#define MOTOR3DIR PEout(14)	
#define MOTOR3 PEout(15)	 

void motors_init(void);												//所有电机端口初始化
void setOrigin(unsigned char id);									//设置电机原点
void motorSetEn(unsigned char id, unsigned char flag);				//设置电机使能
void motorSetDir(unsigned char id,unsigned char flag);				//设置电机转动方向
u32 getMotorPulse(unsigned char id);								//获取电机当前脉冲数
u32 getMotorEndPulse(unsigned char id);
u32 getMotorSpeed(unsigned char id);
void setMotorPos_abs(unsigned char id, u32 pulse);				//设置电机绝对脉冲数
void setMotorPos_rela(unsigned char id,int pulse);					//设置电机相对脉冲数
void setMotorSpeed(unsigned char id, u16 speed);
void setPositiveLimit(unsigned char id, u32 limit);
void setNegativeLimit(unsigned char id, u32 limit);
static void checkLImit(void);
static void setMotorPulse(void);
void motorTaskGo(void);

#endif
