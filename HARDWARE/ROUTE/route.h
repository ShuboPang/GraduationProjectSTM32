#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "sys.h"
#include "motor.h"

#define CALIB_HEIGH_POINT 5			//�궨�߶���Ҫ�ĵ���
#define DISTANCE_PRECISION 1        //������С����

static u32 g_speed = 0;			//�Զ�״̬ȫ���ٶ�

static u32 calibration_heigh[CALIB_HEIGH_POINT][MOTOR_NUM] = { 0 };			//�궨�߶�5���������

u8 checkIsCalib();		//����Ƿ�궨��

void set_calibration_heigh(u8 point);

u32 twoPoint_line(u32 x, u32 x1, u32 y1, u32 x2, u32 y2);
void fixedHeight(u32 distance);	//�̶��߶�

#endif // !_ROUTE_H_
