#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "sys.h"
#include "motor.h"

#define CALIB_HEIGH_POINT 5			//标定高度需要的点数
#define DISTANCE_PRECISION 1        //距离最小量程

static u32 g_speed = 0;			//自动状态全局速度

static u32 calibration_heigh[CALIB_HEIGH_POINT][MOTOR_NUM] = { 0 };			//标定高度5个点的脉冲

u8 checkIsCalib();		//检查是否标定过

void set_calibration_heigh(u8 point);

u32 twoPoint_line(u32 x, u32 x1, u32 y1, u32 x2, u32 y2);
void fixedHeight(u32 distance);	//固定高度

#endif // !_ROUTE_H_
