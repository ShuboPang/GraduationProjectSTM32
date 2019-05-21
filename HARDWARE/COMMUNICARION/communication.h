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
	MOTOR1_P_LIMIT,
	MOTOR1_N_LIMIT,

	//电机2
	MOTOR2_TURN_RIGHT = 11,
	MOTOR2_TURN_LEFT,
	MOTOR2_SPEED,
	MOTOR2_POS,
	MOTOR2_ORIGIN,
	MOTOR2_P_LIMIT,
	MOTOR2_N_LIMIT,

	//电机3
	MOTOR3_TURN_RIGHT = 21,
	MOTOR3_TURN_LEFT,
	MOTOR3_SPEED,
	MOTOR3_POS,
	MOTOR3_ORIGIN,
	MOTOR3_P_LIMIT,
	MOTOR3_N_LIMIT,

	//mpu6050
	MPU6050_ORIGIN = 31,

	//紧急停止
	STOP = 41,

	//标定模式
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

	//自动模式
	AUTO_1 = 71,
	AUTO_2,
	AUTO_3,
	AUTO_4,
	AUTO_5,
	AUTO_6,
	AUTO_G_SPEED,//设置全局速度
	AUTO_BACK,	//是否返回
	AUTO_CYCLE,	//是否循环
	AUTO_CYCLE_TIMES,//循环次数
	AUTO_SYMMETRY,	//是否对称
}controlMode;


void averageFiltering(void);		//平均值滤波
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
