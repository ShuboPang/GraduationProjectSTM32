#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "sys.h"

#define MAX_LENGTH 8
#define MAX_COUNT 5

static u32 s_buff[MAX_COUNT][MAX_LENGTH] = { 0 };
static u32 r_buff[MAX_COUNT][MAX_LENGTH] = { 0 };
extern u8  USART_RX_BUF[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

typedef enum {
	MOTOR1_MODE,
	MOTOR2_MODE,
	MOTOR3_MODE,
	MPU6050_MODE,
	ERROR_MODE,
	MAX
}mode;

typedef enum {
	//���1
	MOTOR1_TURN_RIGHT = 1,
	MOTOR1_TURN_LEFT,
	MOTOR1_SPEED,
	MOTOR1_POS,
	MOTOR1_ORIGIN,
	MOTOR1_P_LIMIT,
	MOTOR1_N_LIMIT,

	//���2
	MOTOR2_TURN_RIGHT = 11,
	MOTOR2_TURN_LEFT,
	MOTOR2_SPEED,
	MOTOR2_POS,
	MOTOR2_ORIGIN,
	MOTOR2_P_LIMIT,
	MOTOR2_N_LIMIT,

	//���3
	MOTOR3_TURN_RIGHT = 21,
	MOTOR3_TURN_LEFT,
	MOTOR3_SPEED,
	MOTOR3_POS,
	MOTOR3_ORIGIN,
	MOTOR3_P_LIMIT,
	MOTOR3_N_LIMIT,

	//mpu6050
	MPU6050_ORIGIN = 31,

	//����ֹͣ
	STOP = 41,

	//�궨ģʽ
	CALIBRATION_1 = 51,
	CALIBRATION_2,
	CALIBRATION_3,
	CALIBRATION_4,
	CALIBRATION_5,
	CALIBRATION_6,
	CALIBRATION_7,
	CALIBRATION_8,
	CALIBRATION_9,
	CALIBRATION_10,
	CALIBRATION_11,

	//�Զ�ģʽ
	AUTO_1 = 71,
	AUTO_2,
	AUTO_3,
	AUTO_4,
	AUTO_5,
	AUTO_6,
	AUTO_G_SPEED,//����ȫ���ٶ�
	AUTO_BACK,	//�Ƿ񷵻�
	AUTO_CYCLE,	//�Ƿ�ѭ��
	AUTO_CYCLE_TIMES,//ѭ������
	AUTO_SYMMETRY,	//�Ƿ�Գ�
}controlMode;


void averageFiltering(void);		//ƽ��ֵ�˲�
u32 getMpu6050_x(void);
u32 getMpu6050_y(void);
u32 getHeight(void);

u32 checkSum(char *src);
void setDistance(u32 dis);
void setAngleOrgin(void);
void getMotorInfo(void);
void getMpu6050(void);
void getError(void);
void setSendBuff(void);
void sendTo(u32 times);
void recv(void);




#endif // !_COMMUNICATION_H_
