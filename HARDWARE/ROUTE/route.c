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

static u32 calibration_angle[CALIB_ANGLE_POINT][MOTOR_NUM] = { 0 };			//标定角度6个点的脉冲

#define MAX_COUNT 256

//需要经过的目标位置
static struct route_pulse
{
	u8 count;
	u32 pulse[3][MAX_COUNT];
}Route_pulse;

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

//开始自动模式
void startAutoMode()
{
	isAuto = 1;
}

//停止自动模式  会将当前正在运行的位置运行完成后结束，不会立即停止
void endAutoMode()
{
	isAuto = 0;
}

/***********************************************************/

//设置单个轴的目标位置
void set_route_one_pulse(u8 i,u8 id, u32 pulse)
{
	if (i >= MOTOR_START_NUM && i <= MOTOR_NUM)
	{
		Route_pulse.pulse[id - MOTOR_START_NUM][i + MOTOR_START_NUM] = pulse;
	}
}

//重置路径脉冲位置
void reset_route()
{
	Route_pulse.count = 0;			//脉冲计数置零，就会重新覆盖位置
}

//在路径规划中添加脉冲位置
void route_add(u32 *pulse)
{
	Route_pulse.count++;
	Route_pulse.pulse[0][Route_pulse.count - 1] = pulse[0];
	Route_pulse.pulse[1][Route_pulse.count - 1] = pulse[1];
	Route_pulse.pulse[2][Route_pulse.count - 1] = pulse[2];
}

//将规划后的目标脉冲依次运动
void route_setPulse()
{
	static u32 i = 0;
	static u32 count = 0;		//循环计数
	if (isAuto)
	{
		if (getMotorsIsEnd()&&(i != Route_pulse.count))
		{
			for (u8 j = 0; j < MOTOR_NUM; j++)
			{
				setMotorPos_abs(j + MOTOR_START_NUM, Route_pulse.pulse[j][i]);
			}
			speed_Planning();
			i++;
		}
		//轨迹位置发送完成
		else if (i >= Route_pulse.count)
		{
			//判断运动是否结束，结束之后才可以发送下一个点的位置
			if (getMotorsIsEnd())
			{
				//判断是否开启循环
				if (getCycle())
				{
					//判断是否对称运动
					if (getSymmetry())
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							u32 cur_pulse = getMotorPulse(j + MOTOR_START_NUM);
							setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]*2 - cur_pulse);
						}
						speed_Planning();
					}
					else
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]);
						}
						speed_Planning();
					}

					if (count >= getCycleTimes())
					{
						isAuto = 0;
						count = 0;
					}
					else
					{
						count++;
					}

				}
				else
				{
					//结束自动运行
					isAuto = 0;
					count = 0;
				}
				i = 0;

				//自动运行轨迹结束
				if (isAuto == 0)
				{
					
					if (getComeBack())
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							if (getMotorPulse(j + MOTOR_START_NUM) != calibration_heigh[1][j])
							{
								setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]);
							}
							speed_Planning();
						}
					}
				}
			}
		}
	}
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


//设置高度标定点的脉冲位置值
void set_calibration_heigh(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//设置角度标定点的脉冲位置值
void set_calibration_angle(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//速度规划函数
//通过比较每个轴的目标脉冲与当前脉冲差，使所有轴同时到达指定位置
void speed_Planning(void)
{
	u32 x_speed = getG_speed();
	u32 y_speed = 0;
	u32 z_speed = 0;
	setMotorSpeed(X_MOTOR, x_speed);		//将全局速度设置给x轴
	
	if (getMotorPulse(X_MOTOR) == getMotorEndPulse(X_MOTOR))
	{
		y_speed = getG_speed();
		if (getMotorPulse(Y_MOTOR) == getMotorEndPulse(Y_MOTOR))
		{
			z_speed = getG_speed();
		}
		else
		{
			z_speed = abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)))*y_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
		}
	}
	else
	{
		y_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)));
		z_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
	}
	if (x_speed < 10)
	{
		x_speed = 10;
	}
	if (y_speed < 10)
	{
		y_speed = 10;
	}
	if (z_speed < 10)
	{
		z_speed = 10;
	}
	setMotorSpeed(Y_MOTOR, y_speed);
	setMotorSpeed(Z_MOTOR, z_speed);
}


//固定高度
void fixedHeight(u32 distance)
{
	u32 ret = 0;
	u32 pulse_tmp[3] = { 0 };
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
		pulse_tmp[i] = ret;
//		setMotorPos_abs(i + MOTOR_START_NUM, ret);
	}
	route_add(pulse_tmp);
}

//固定角度
void fixedAngle(u32 angle)
{
	u32 pulse_tmp[3] = { 0 };
	/*
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (angle < 30 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 0, calibration_heigh[1][i], 5 * DISTANCE_PRECISION, calibration_angle[1][i]);
		}
		else if (angle >= 30 * DISTANCE_PRECISION&&angle <= 60 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 30 * DISTANCE_PRECISION, calibration_angle[1][i], 60 * DISTANCE_PRECISION, calibration_angle[3][i]);
		}
		else if (angle >= 60* DISTANCE_PRECISION&&angle <= 120 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 60 * DISTANCE_PRECISION, calibration_angle[3][i], 120 * DISTANCE_PRECISION, calibration_angle[4][i]);
		}
		else if (angle >= 120 * DISTANCE_PRECISION&&angle <= 150 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 120 * DISTANCE_PRECISION, calibration_angle[4][i], 150 * DISTANCE_PRECISION, calibration_angle[5][i]);
		}
		else
		{
			pulse_tmp[i] = getMotorPulse(i + MOTOR_START_NUM);
		}
	}
	*/
	pulse_tmp[0] = getMotorPulse(X_MOTOR);
	pulse_tmp[1] = -0.0644*((float)angle)*((float)angle)+16.22*((float)angle)-393+ calibration_heigh[1][1];
	pulse_tmp[2] = -0.081*((float)angle)*((float)angle) + 9.2965*((float)angle) +373 + calibration_heigh[1][1];

	route_add(pulse_tmp);
}

//固定长度
void fixedLength(u32 data)
{
	u32 pulse_tmp[3] = { 0 };
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (data < 5 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 0, calibration_heigh[1][i], 5 * DISTANCE_PRECISION, calibration_angle[0][i]);
		}
		else if (data >= 5 * DISTANCE_PRECISION&&data <= 10 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 5 * DISTANCE_PRECISION, calibration_angle[0][i], 10 * DISTANCE_PRECISION, calibration_angle[1][i]);
		}
		else if (data >= 10 * DISTANCE_PRECISION&&data <= 15 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 10 * DISTANCE_PRECISION, calibration_angle[1][i], 15 * DISTANCE_PRECISION, calibration_angle[2][i]);
		}
		else
		{
			pulse_tmp[i] = getMotorPulse(i+ MOTOR_START_NUM);
		}
	}
	route_add(pulse_tmp);
}