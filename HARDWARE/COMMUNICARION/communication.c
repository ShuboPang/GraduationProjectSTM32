#include "communication.h"

u8 checkSum(char *src,u32 length)
{
	u32 sum = 0;
	for (u32 i = 0; i < length; i++)
	{
		sum += src[i];
	}
	return sum & 0xff;
}



void setSendBuff() 
{
	//��ʼ����ʼ��־λ�ͽ�����־λ
	for (u8 i = 0; i < MAX_COUNT; i++)
	{
		s_buff[i][0] = 0x55;
		s_buff[i][MAX_LENGTH-1] = 0x56;
	}
	// ��ʼλ��1������ţ�1����ǰ���壺4��Ŀ������ ��4 ���ٶȣ�4��У��ͣ�1������λ��1
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		s_buff[i][1] = i + 1;			//���õ����

		//���õ�ǰ������
		u32 currentPulse =  getMotorPulse(i);
		s_buff[i][2] = currentPulse >> 24;
		s_buff[i][3] = currentPulse >> 16;
		s_buff[i][4] = currentPulse >> 8;
		s_buff[i][5] = currentPulse >> 0;

		//����Ŀ��������
		u32 endPulse = getMotorEndPulse(i);
		s_buff[i][6] = endPulse >> 24;
		s_buff[i][7] = endPulse >> 16;
		s_buff[i][8] = endPulse >> 8;
		s_buff[i][9] = endPulse >> 0;

		u32 speed = getMotorSpeed(i);
		s_buff[i][10] = speed >> 24;
		s_buff[i][11] = speed >> 16;
		s_buff[i][12] = speed >> 8;
		s_buff[i][13] = speed >> 0;

		s_buff[i][14] = checkSum(s_buff[i], 14);
	}
}

void sendTo()
{
	static u8 count = 0;
	static u8 i = 0;
	i++;
	if (i >= 10)
	{
		printf("%s", s_buff[count]);
		count++;
		if (count >= MAX_COUNT)
		{
			count = 0;
		}
		i = 0;
	}
}