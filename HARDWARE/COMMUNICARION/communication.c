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
	//��ʼ����ʼ��־λ�ͽ�����־λ
	for (u8 i = 0; i < MAX_COUNT; i++)
	{
		s_buff[i][0] = 123;
		s_buff[i][MAX_LENGTH-1] = 321;
	}
	// ��ʼλ��1������ţ�1����ǰ���壺1��Ŀ������ ��1 ���ٶȣ�1��rec:1;У��ͣ�1������λ��1
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		s_buff[i][1] = i + 1;			//���õ����

		//���õ�ǰ������
		u32 currentPulse =  getMotorPulse(i);
		s_buff[i][2] = currentPulse;

		//����Ŀ��������
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