#include "communication.h"
#include "usart.h"
#include "motor.h"
#include "error.h"
#include "route.h"
#include "JY901.h"
#include "pca9685.h"

#define FILTER_TIMES 5


static u32 angleInfo[] = { 0,0,0 };   // 陀螺仪偏差值，超声波实时值
static u32 angleFilterInfo[] = { 0,0,0 };	//滤波之后的值

static int32 tmp[3][FILTER_TIMES] = { 0 };

//平均值滤波
void averageFiltering()
{
	static u8 count[3] = { 0 };
	//x
	if (count[0] != (FILTER_TIMES-1))
	{
		tmp[0][count[0]] = stcAngle.Angle[0];
		count[0]++;
	}
	else
	{
		for (u8 i = 0; i < (FILTER_TIMES - 1); i++)
		{
			tmp[0][i] = tmp[0][i + 1];
		}
		tmp[0][count[0]] = stcAngle.Angle[0];
	}
	
	//y
	u8 y = 1;
	if (count[y] != (FILTER_TIMES - 1))
	{
		tmp[y][count[y]] = stcAngle.Angle[y];
		count[y]++;
	}
	else
	{
		for (u8 i = 0; i < (FILTER_TIMES - 1); i++)
		{
			tmp[y][i] = tmp[y][i + 1];
		}
		tmp[y][count[y]] = stcAngle.Angle[y];
	}

	//z
	 y = 2;
	if (count[y] != (FILTER_TIMES - 1))
	{
		tmp[y][count[y]] = angleInfo[2];
		count[y]++;
	}
	else
	{
		for (u8 i = 0; i < (FILTER_TIMES - 1); i++)
		{
			tmp[y][i] = tmp[y][i + 1];
		}
		tmp[y][count[y]] = angleInfo[2];
	}

	int32 sum[3] = { 0 };
	for (u8 i = 0; i < 3; i++)
	{
		for (u8 j = 0; j < FILTER_TIMES; j++)
		{
			sum[i] += tmp[i][j];
		}
		angleFilterInfo[i] = sum[i] / FILTER_TIMES;
	}

}


//设置角度原点
void setAngleOrgin()
{
	angleInfo[0] = angleFilterInfo[0];
	angleInfo[1] = angleFilterInfo[1];
}

void setDistance(u32 dis)
{
	angleInfo[2] = dis;
}


//获取mpu6050 x轴偏角
u32 getMpu6050_x()
{
	return angleFilterInfo[0]- angleInfo[0];
}


//获取mpu6050 y轴偏角
u32 getMpu6050_y()
{
	return angleFilterInfo[1] - angleInfo[1];
}


//获取高度值
u32 getHeight()
{
	return angleFilterInfo[2];
}


u32 checkSum(char *src)
{
	u32 sum = 0;
	for (u32 i = 0; i < (MAX_LENGTH - 2); i++)
	{
		sum += src[i];
	}
	return sum;
}

void getMotorInfo()
{
	// 起始位：1；电机号：1；当前脉冲：1；目标脉冲 ：1 ；速度：1；rev:1; 校验和：1；结束位：1
	for (u8 i = MOTOR1_MODE; i < MOTOR_NUM; i++)
	{
		s_buff[i][1] = i + 1;			//设置电机号
		s_buff[i][2] = getMotorPulse(i+1);
		s_buff[i][3] = getMotorEndPulse(i+1);
		s_buff[i][4] = getMotorSpeed(i+1);
		s_buff[i][5] = 0;
	}
}

void getMpu6050()
{
	//起始位：1；
	//类型：陀螺仪
	//x：1
	//y ：1 
	//z：1；
	//rev:1; 
	//校验和：1；
	//结束位：1
	s_buff[MPU6050_MODE][1] = MPU6050_MODE + 1;			

	averageFiltering();
	s_buff[MPU6050_MODE][2] = getMpu6050_x();
	s_buff[MPU6050_MODE][3] = getMpu6050_y();
	s_buff[MPU6050_MODE][4] = getHeight();
	s_buff[MPU6050_MODE][5] = 0;
	
}

void getError()
{
	//起始位
	//类型：报警
	//报警号
	//rev
	//rev
	//rev:1; 
	//校验和：1；
	//结束位：1
	s_buff[ERROR_MODE][1] = ERROR_MODE + 1;

	s_buff[ERROR_MODE][2] = getErrorState();
	s_buff[ERROR_MODE][3] = 0;
	s_buff[ERROR_MODE][4] = 0;
	s_buff[ERROR_MODE][5] = 0;

}


void setSendBuff() 
{
	//初始化起始标志位和结束标志位
	for (u8 i = 0; i < MAX_COUNT; i++)
	{
		s_buff[i][0] = 123;
		s_buff[i][MAX_LENGTH-1] = 321;
	}
	
	getMotorInfo();
	getMpu6050();
	getError();
	//校验和计算
	for (u8 i = 0; i < MAX_COUNT; i++) 
	{
		s_buff[i][MAX_LENGTH - 2] = checkSum(s_buff[i]);
	}
}

void sendTo(u32 times)
{
	static u8 count = 0;
	static u8 i = 0;
	i++;
	if (count == 0)
	{
		setSendBuff();
	}
	if (i >= times)
	{
		printf("#%d,%d,%d,%d,%d,%d,%d,%d", s_buff[count][0],s_buff[count][1], s_buff[count][2],
										 s_buff[count][3], s_buff[count][4], s_buff[count][5],
										 s_buff[count][6], s_buff[count][7]);
		count++;
		if (count >= MAX_COUNT)
		{
			count = 0;
		}
		i = 0;
	}
}

void recv()
{
	u8 i = 0;
	u32 data = ((u32)USART_RX_BUF[2] << 24) + ((u32)USART_RX_BUF[3] << 16) + ((u32)USART_RX_BUF[4] << 8) + ((u32)USART_RX_BUF[5]);

	i = USART_RX_BUF[1];
	u16 pwm = calculate_PWM(data);
	setPWM(i, 0, pwm);
}
