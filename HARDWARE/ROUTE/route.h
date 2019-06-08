#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "sys.h"
#include "motor.h"

#define CALIB_HEIGH_POINT 5			//标定高度需要的点数
#define CALIB_ANGLE_POINT 6			//标定角度需要的点数
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

//将规划后的目标脉冲依次运动
void route_setPulse();

//在路径规划中添加脉冲位置
void route_add(u32 *pulse);

//返回需要运动的位置数量
u32 get_route_count();

//重置路径脉冲位置
void reset_route();

//设置单个轴的目标位置
void set_route_one_pulse(u8 i, u8 id, u32 pulse);

//开始自动模式
void startAutoMode();

//停止自动模式  会将当前正在运行的位置运行完成后结束，不会立即停止
void endAutoMode();

void speed_Planning(void);		//速度规划函数
void fixedHeight(u32 distance);	//固定高度
void fixedAngle(u32 angle);		//固定角度
void fixedLength(u32 data);		//固定长度
								
void circle(u32 radius);			//画圆
#endif // !_ROUTE_H_
