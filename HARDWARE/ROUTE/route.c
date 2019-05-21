#include "route.h"
#include "communication.h"
#include "error.h"
#include "motor.h"
#include "math.h"


static u32 g_speed = 0;			//自动状态全局速度

static u8 isComeBack = 0;	//是否返回
static u8 isCycle = 0;		//是否循环
static u8 isSymmetry = 0;	//是否对称
static u32 cycle_times = 0;	//循环次数

static u8 isAuto = 0;//是否处于自动模式

static u32 calibration_heigh[CALIB_HEIGH_POINT][MOTOR_NUM] = { 0 };			//标定高度5个点的脉冲


/*********************  设置自动运行时的参数  ******************************/

//设置全局速度
void setG_speed(u32 speed)
{
	g_speed = speed;
}

//获取全局速度
u32 getG_speed()
{
	return g_speed;
}

//设置是否返回
void setComeBack(u8 state)
{
	isComeBack = state;
}

//获取是否返回
u8 getComeBack()
{
	return isComeBack;
}

//设置是否循环
void setCycle(u8 state)
{
	isCycle = state;
}

//获取是否循环
u8 getCycle()
{
	return isCycle;
}

//设置循环次数
void setCycleTimes(u32 times)
{
	cycle_times = times;
}

//获取循环次数
u8 getCycleTimes()
{
	return cycle_times;
}

//设置是否对称
void setSymmetrye(u8 state)
{
	isSymmetry = state;
}

//获取是否对称
u8 getSymmetry()
{
	return isSymmetry;
}

/***********************************************************/


//检查是否标定过 0:未标定或有错；1：标定过
u8 checkIsCalib()		
{
	for (u8 i = 0; i < CALIB_HEIGH_POINT-1; i++)
	{
		for (u8 j = 0; j < MOTOR_NUM; j++)
		{
			if (calibration_heigh[i][j] >= calibration_heigh[i + 1][j])
			{
				setErrorState(NOT_CALIBRATION);
				return 0;
			}
		}
	}
	return 1;
}


//两点法求直线方程
u32 twoPoint_line(u32 x,u32 x1, u32 y1, u32 x2, u32 y2)
{
	return (y2 - y1) / (x2 - x1)*(x - x1) + y1;
}


//设置标定点的脉冲位置值
void set_calibration_heigh(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//速度规划函数
//通过比较每个轴的目标脉冲与当前脉冲差，使所有轴同时到达指定位置
void speed_Planning()
{
	u32 x_speed = getG_speed();
	setMotorSpeed(X_MOTOR, x_speed);		//将全局速度设置给x轴
	u32 y_speed = 0;
	y_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)));
	setMotorSpeed(Y_MOTOR, y_speed);
	u32 z_speed = 0;
	z_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
	setMotorSpeed(Z_MOTOR, z_speed);
}


//固定高度
void fixedHeight(u32 distance)
{
	u32 ret = 0;
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (distance < 5 * DISTANCE_PRECISION)
		{
			ret = 0;
		}
		else if (distance >= 5 * DISTANCE_PRECISION&&distance <= 10 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 5 * DISTANCE_PRECISION, calibration_heigh[0][i], 10 * DISTANCE_PRECISION, calibration_heigh[1][i]);
		}
		else if (distance >= 10 * DISTANCE_PRECISION&&distance <= 15 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 10 * DISTANCE_PRECISION,  calibration_heigh[1][i], 15 * DISTANCE_PRECISION, calibration_heigh[2][i]);
		}
		else if (distance >= 15 * DISTANCE_PRECISION&&distance <= 20 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 15 * DISTANCE_PRECISION,  calibration_heigh[2][i], 20 * DISTANCE_PRECISION, calibration_heigh[3][i]);
		}
		else if (distance >= 20 * DISTANCE_PRECISION&&distance <= 25 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 20 * DISTANCE_PRECISION,  calibration_heigh[3][i], 25 * DISTANCE_PRECISION, calibration_heigh[4][i]);
		}
		else
		{
			ret = 0;
		}
		setMotorPos_abs(i + MOTOR_START_NUM, ret);
	}
	isAuto = 1;
	speed_Planning();
}
