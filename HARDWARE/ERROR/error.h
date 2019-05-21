#ifndef _ERROR_H_
#define _ERROR_H_

#include "sys.h"

typedef enum
{
	NONE,
	SYSTEM_ERROR = 1,
	NOT_CALIBRATION = 2,
	MOVE_1_PositiveLimit = 10,		//��1�˶�����������
	MOVE_1_NegativeLimit,
	MOTOR_1_POS_PositiveLimit,		//��1Ŀ��ֵ����������
	MOTOR_1_POS_NegativeLimit,
	MOVE_2_PositiveLimit = 20,
	MOVE_2_NegativeLimit,
	MOTOR_2_POS_PositiveLimit,
	MOTOR_2_POS_NegativeLimit,
	MOVE_3_PositiveLimit = 30,
	MOVE_3_NegativeLimit,
	MOTOR_3_POS_PositiveLimit,
	MOTOR_3_POS_NegativeLimit,

}ERRORType;

void setErrorState(ERRORType i);
ERRORType getErrorState(void);


#endif // !_ERROR_H_
