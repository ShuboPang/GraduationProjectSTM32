#include "route.h"
#include "communication.h"
#include "error.h"
#include "motor.h"


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


//固定高度
void fixedHeight(u32 distance)
{
	distance *= 10;			//将距离转化为mm
	u32 ret = 0;
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (distance < 5 * DISTANCE_PRECISION)
		{
			ret = 0;
		}
		else if (distance >= 5 * DISTANCE_PRECISION&&distance <= 10 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 5 * DISTANCE_PRECISION, 10 * DISTANCE_PRECISION, calibration_heigh[0][i], calibration_heigh[1][i]);
		}
		else if (distance >= 10 * DISTANCE_PRECISION&&distance <= 15 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 10 * DISTANCE_PRECISION, 15 * DISTANCE_PRECISION, calibration_heigh[1][i], calibration_heigh[2][i]);
		}
		else if (distance >= 15 * DISTANCE_PRECISION&&distance <= 20 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 15 * DISTANCE_PRECISION, 20 * DISTANCE_PRECISION, calibration_heigh[2][i], calibration_heigh[3][i]);
		}
		else if (distance >= 20 * DISTANCE_PRECISION&&distance <= 25 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 20 * DISTANCE_PRECISION, 25 * DISTANCE_PRECISION, calibration_heigh[3][i], calibration_heigh[4][i]);
		}
		else
		{
			ret = 0;
		}
		setMotorPos_abs(i + MOTOR_START_NUM, ret);
	}
}