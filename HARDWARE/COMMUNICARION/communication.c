#include "communication.h"
#include "usart.h"
#include "motor.h"

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
	// ��ʼλ��1������ţ�1����ǰ���壺1��Ŀ������ ��1 ���ٶȣ�1��rev:1; У��ͣ�1������λ��1
	for (u8 i = MOTOR1_MODE; i < MOTOR_NUM; i++)
	{
		s_buff[i][1] = i + 1;			//���õ����
		s_buff[i][2] = getMotorPulse(i+1);
		s_buff[i][3] = getMotorEndPulse(i+1);
		s_buff[i][4] = getMotorSpeed(i+1);
		s_buff[i][5] = 0;
	}
}

void getMpu6050()
{
	//��ʼλ��1��
	//���ͣ�������
	//x��1
	//y ��1 
	//z��1��
	//rev:1; 
	//У��ͣ�1��
	//����λ��1

	s_buff[MPU6050_MODE][1] = MPU6050_MODE + 1;			

	s_buff[MPU6050_MODE][2] = 0;
	s_buff[MPU6050_MODE][3] = 0;
	s_buff[MPU6050_MODE][4] = 0;
	s_buff[MPU6050_MODE][5] = 0;
	
}

void getError()
{
	//��ʼλ
	//���ͣ�����
	//������
	//rev
	//rev
	//rev:1; 
	//У��ͣ�1��
	//����λ��1
	s_buff[ERROR_MODE][1] = ERROR_MODE + 1;

	s_buff[ERROR_MODE][2] = 1;
	s_buff[ERROR_MODE][3] = 0;
	s_buff[ERROR_MODE][4] = 0;
	s_buff[ERROR_MODE][5] = 0;

}


void setSendBuff() 
{
	//��ʼ����ʼ��־λ�ͽ�����־λ
	for (u8 i = 0; i < MAX_COUNT; i++)
	{
		s_buff[i][0] = 123;
		s_buff[i][MAX_LENGTH-1] = 321;
	}
	
	getMotorInfo();
	getMpu6050();
	getError();
	//У��ͼ���
	for (u8 i = 0; i < MAX_COUNT; i++) 
	{
		s_buff[i][MAX_LENGTH - 2] = checkSum(s_buff[i]);
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

void recv()
{
	u8 i = 0;
	u32 data = ((u32)USART_RX_BUF[2] << 24) + ((u32)USART_RX_BUF[3] << 16) + ((u32)USART_RX_BUF[4] << 8) + ((u32)USART_RX_BUF[5]);

	i = USART_RX_BUF[1];
	switch (i)
	{

	case MOTOR1_TURN_RIGHT:
	case MOTOR2_TURN_RIGHT:
	case MOTOR3_TURN_RIGHT:
		setMotorPos_rela(i / 10+1, data);
		break;

	case MOTOR1_TURN_LEFT:
	case MOTOR2_TURN_LEFT:
	case MOTOR3_TURN_LEFT:
		setMotorPos_rela(i / 10+1, data*-1);
		break;
		
	case MOTOR1_SPEED:
	case MOTOR2_SPEED:
	case MOTOR3_SPEED:
		setMotorSpeed(i / 10+1, data);
		break;

	case MOTOR1_POS:
	case MOTOR2_POS:
	case MOTOR3_POS:
		setMotorPos_abs(i / 10+1, data);
		break;

	case MOTOR1_ORIGIN:
	case MOTOR2_ORIGIN:
	case MOTOR3_ORIGIN:
		setOrigin(i / 10 + 1);
		break;

	default:
		break;
	}
}