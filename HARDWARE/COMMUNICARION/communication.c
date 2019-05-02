#include "communication.h"

u32 checkSum(char *src)
{
	u32 sum = 0;
	for (u32 i = 0; i < (MAX_LENGTH - 2); i++)
	{
		sum += src[i];
	}
	return sum;
}



void setSendBuff() 
{
	//初始化起始标志位和结束标志位
	for (u8 i = 0; i < MAX_COUNT; i++)
	{
		s_buff[i][0] = 123;
		s_buff[i][MAX_LENGTH-1] = 321;
	}
	// 起始位：1；电机号：1；当前脉冲：1；目标脉冲 ：1 ；速度：1；rec:1;校验和：1；结束位：1
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		s_buff[i][1] = i + 1;			//设置电机号

		//设置当前脉冲数
		u32 currentPulse =  getMotorPulse(i);
		s_buff[i][2] = currentPulse;

		//设置目标脉冲数
		u32 endPulse = getMotorEndPulse(i);
		s_buff[i][3] = endPulse >> 24;

		u32 speed = getMotorSpeed(i);
		s_buff[i][4] = speed >> 24;

		s_buff[i][MOTOR_NUM-2] = checkSum(s_buff[i]);
	}
}

void sendTo()
{
	static u8 count = 0;
	static u8 i = 0;
	i++;
	if (count == 0)
	{
		setSendBuff();
	}
	if (i >= 10)
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