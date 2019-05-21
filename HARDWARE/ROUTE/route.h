#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "sys.h"
#include "motor.h"

#define CALIB_HEIGH_POINT 5			//标定高度需要的点数
#define DISTANCE_PRECISION 1        //距离最小量程

/************************ 设置自动运行的参数  *************************/
void setG_speed(u32 speed);

u32 getG_speed(void);

//设置是否返回
void setComeBack(u8 state);

//获取是否返回
u8 getComeBack(void);

//设置是否循环
void setCycle(u8 state);

//获取是否循环
u8 getCycle(void);

//设置循环次数
void setCycleTimes(u32 times);

//获取循环次数
u8 getCycleTimes(void);

//设置是否对称
void setSymmetrye(u8 state);

//获取是否对称
u8 getSymmetry(void);

/***************************************************/


u8 checkIsCalib(void);		//检查是否标定过

void set_calibration_heigh(u8 point);

u32 twoPoint_line(u32 x, u32 x1, u32 y1, u32 x2, u32 y2);
/*****************************************************/

void speed_Planning(void);//速度规划函数
void fixedHeight(u32 distance);	//固定高度

#endif // !_ROUTE_H_
