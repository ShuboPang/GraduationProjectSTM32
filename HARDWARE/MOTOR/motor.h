#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//motor��������	   
//STM32F4����-�⺯���汾								  
////////////////////////////////////////////////////////////////////////////////// 	

#define MOTOR_NUM 3
#define MOTOR_START_NUM 1

//����˿ڶ���
#define MOTOR1EN PEout(7)	
#define MOTOR1DIR PEout(8)	
#define MOTOR1 PEout(9)	 
#define MOTOR2EN PEout(10)	
#define MOTOR2DIR PEout(11)	
#define MOTOR2 PEout(12)
#define MOTOR3EN PEout(13)	
#define MOTOR3DIR PEout(14)	
#define MOTOR3 PEout(15)	 

void motors_init(void);												//���е���˿ڳ�ʼ��
void setOrigin(unsigned char id);									//���õ��ԭ��
void motorSetEn(unsigned char id, unsigned char flag);				//���õ��ʹ��
void motorSetDir(unsigned char id,unsigned char flag);				//���õ��ת������
u32 getMotorPulse(unsigned char id);								//��ȡ�����ǰ������
u32 getMotorEndPulse(unsigned char id);
u32 getMotorSpeed(unsigned char id);
void setMotorPos_abs(unsigned char id, u32 pulse);				//���õ������������
void setMotorPos_rela(unsigned char id,int pulse);					//���õ�����������
void setMotorSpeed(unsigned char id, u16 speed);
void setPositiveLimit(unsigned char id, u32 limit);
void setNegativeLimit(unsigned char id, u32 limit);
static void checkLImit(void);
static void setMotorPulse(void);
void motorTaskGo(void);

#endif
