#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "sys.h"

#define MAX_LENGTH 8
#define MAX_COUNT 5

static u32 s_buff[MAX_COUNT][MAX_LENGTH] = { 0 };
static u32 r_buff[MAX_COUNT][MAX_LENGTH] = { 0 };
extern u8  USART_RX_BUF[200]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

typedef enum {
	MOTOR1_MODE,
	MOTOR2_MODE,
	MOTOR3_MODE,
	MPU6050_MODE,
	ERROR_MODE,
	MAX
}mode;

typedef enum {
	//电机1
	MOTOR1_TURN_RIGHT = 1,
	MOTOR1_TURN_LEFT,
	MOTOR1_SPEED,
	MOTOR1_POS,
	MOTOR1_ORIGIN,

	//电机2
	MOTOR2_TURN_RIGHT = 11,
	MOTOR2_TURN_LEFT,
	MOTOR2_SPEED,
	MOTOR2_POS,
	MOTOR2_ORIGIN,

	//电机3
	MOTOR3_TURN_RIGHT = 21,
	MOTOR3_TURN_LEFT,
	MOTOR3_SPEED,
	MOTOR3_POS,
	MOTOR3_ORIGIN,

	//mpu6050
	MPU6050_ORIGIN = 31,

	//紧急停止
	STOP = 41,

	//标定模式
	CALIBRATION = 51,


	//自动模式
	AUTO_1 = 61,
	AUTO_2,
	AUTO_3,
	AUTO_4,
	AUTO_5,
	AUTO_6,
	AUTO_7,
}controlMode;


u32 checkSum(char *src);
void getMotorInfo();
void getMpu6050();
void getError();
void setSendBuff();
void sendTo();
void recv();




#endif // !_COMMUNICATION_H_
