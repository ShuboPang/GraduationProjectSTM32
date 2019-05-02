#include "motor.h"

/*
motorPulse[0] : ʵʱ������ ��
motorPulse[1] : Ŀ�������� ;
motorPulse[2] : ����ٶȣ�
motorPulse[3] : ���� ;
motorPulse[4] : ������ ;
motorPulse[5] : �����ޣ�
*/
static u32 motorPulse[6][MOTOR_NUM] = { 0 };

//���е���˿ڳ�ʼ��
void motors_init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
}

void motorSetEn(unsigned char id, unsigned char flag)
{
	switch (id)
	{
	case 1:
		MOTOR1EN = flag;
		break;
	case 2:
		MOTOR2EN = flag;
		break;
	case 3:
		MOTOR3EN = flag;
		break;
	default:
		break;
	}
}

void setOrigin(unsigned char id)
{
	if (id >= 1 && id <= 3)
	{
		motorPulse[0][id - MOTOR_START_NUM] = 0;
		motorPulse[1][id - MOTOR_START_NUM] = 0;
	}
}

/**

@function�����õ��ת������
@para��
	id������� ��Χ��1-3
	falg��ת������ 0����ת��2����ת
@return��
	��
*/
void motorSetDir(unsigned char id,unsigned char flag)
{

	switch(id)
	{
	case 1:
		MOTOR1DIR = flag;
		break;
	case 2:
		MOTOR2DIR = flag;
		break;
	case 3:
		MOTOR3DIR = flag;
		break;
	default:
		break;
	}
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[3][id - MOTOR_START_NUM] = flag;
	}
}


//��ȡ�����ǰ������
u32 getMotorPulse(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[0][id - MOTOR_START_NUM];
	}
	return 0;
}

//��ȡ���Ŀ��������
u32 getMotorEndPulse(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[1][id - MOTOR_START_NUM];
	}
	return 0;
}

//��ȡ����ٶ�
u32 getMotorSpeed(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[2][id - MOTOR_START_NUM];
	}
	return 0;
}

/*
���õ������λ��
@para��
	id������� ��Χ��1-3
	pulse��Ŀ�������� ������ڵ���0
@return ��
	��

*/
void setMotorPos_abs(unsigned char id,u32 pulse)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		 motorPulse[1][id- MOTOR_START_NUM] =  pulse;
	}
}

/*
���õ�����λ��
@para��
	id������� ��Χ��1-3
	pulse�����Ŀ�������� �����ɸ�
@return ��
	��
*/
void setMotorPos_rela(unsigned char id,int pulse)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[1][id- MOTOR_START_NUM] = motorPulse[0][id- MOTOR_START_NUM] + pulse;
	}
}

//���õ���ٶ�
void setMotorSpeed(unsigned char id, u16 speed)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[2][id- MOTOR_START_NUM] = speed;
	}
}

//����������
void setPositiveLimit(unsigned char id, u32 limit)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[4][id - MOTOR_START_NUM] = limit;
	}
}

//���ø�����
void setNegativeLimit(unsigned char id, u32 limit)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[5][id - MOTOR_START_NUM] = limit;
	}
}

//����Ƿ񳬳�����
static void checkLImit(void)
{
	unsigned char i = 0;
	for (i = 0; i < MOTOR_NUM; i++)
	{
		//���ʵʱֵ�Ƿ񳬳�����
		if (motorPulse[0][i] < motorPulse[5][i] || motorPulse[0][i] > motorPulse[4][i])
		{
			//����
		}
		//���Ŀ��ֵ�Ƿ񳬳�����
		if (motorPulse[1][i] < motorPulse[5][i])
		{
			motorPulse[1][i] = motorPulse[5][i];
		}
		else if (motorPulse[1][i] > motorPulse[4][i])
		{
			motorPulse[1][i] = motorPulse[4][i];
		}
	}
}

//�������� ��ʵʱ����ÿ����ĵ�ʵʱ������
//����ڶ�ʱ���жϺ����ж�ʱִ��
static void setMotorPulse(void)
{
	static u32 count[3] = {0};			//��¼��ǰ�ڼ��ν��뺯��
	u32 speed = 0;
	for (u32 i = 0; i < MOTOR_NUM; i++)
	{
		if (motorPulse[0][i] != motorPulse[1][i])
		{
			speed = motorPulse[2][i];
			if (count[i] < speed)
			{
				switch (i + MOTOR_START_NUM)
				{
				case 1:
					MOTOR1 = 1;
					break;
				case 2:
					MOTOR2 = 1;
					break;
				case 3:
					MOTOR3 = 1;
					break;
				default:
					break;
				}
			}
			else if (count[i] >= speed && count[i] < 2 * speed)
			{
				switch (i + MOTOR_START_NUM)
				{
				case 1:
					MOTOR1 = 0;
					break;
				case 2:
					MOTOR2 = 0;
					break;
				case 3:
					MOTOR3 = 0;
					break;
				default:
					break;
				}
			}
			else
			{
				count[i] = 0;
				if (motorPulse[3][i])
				{
					motorPulse[0][i] += 1;
				}
				else
				{
					motorPulse[0][i] -= 1;
				}
			}
			count[i]++;
		}
	}
}


void motorTaskGo()
{
	unsigned char i = 0;
	checkLImit();
	for ( i = 0; i < MOTOR_NUM; i++)
	{
		//����Ŀ������͵�ǰ�����жϵ��ת������
		if (motorPulse[0][i] <= motorPulse[1][i])
		{
			motorSetDir(i + MOTOR_START_NUM, 0);
		}
		else
		{
			motorSetDir(i + MOTOR_START_NUM, 1);
		}
	}
	setMotorPulse();
}

