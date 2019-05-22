#include "motor.h"
#include "error.h"
#include "led.h"

/*
motorPulse[0] : ʵʱ������ ��
motorPulse[1] : Ŀ�������� ;
motorPulse[2] : ����ٶȣ�
motorPulse[3] : ʵʱת������ ;
motorPulse[4] : ������ ;
motorPulse[5] : �����ޣ�
motorPulse[6] : �����ת����
*/
static u32 motorPulse[7][MOTOR_NUM] = { 0 };


//�˶�������־λ
static u8 motorEnd[MOTOR_NUM] = { 0 };

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
	  for (int i = 1; i <= 3; i++)
	  {
		  motorSetEn(i, 1);
	  }
}


//��ʼ������ٶȺ�λ�ò���
void motor_pos_init()
{
	u8 i = 0;
	for (i = 0; i < MOTOR_NUM; i++)
	{
		motorPulse[0][i] = 5000;
	}
	for (i = 0; i < MOTOR_NUM; i++)
	{
		motorPulse[1][i] = 5000;
	}
	for (i = 0; i < MOTOR_NUM; i++)
	{
		motorPulse[2][i] = 10;
	}
	
}


//���ʹ��
void motorSetEn(unsigned char id, unsigned char flag)
{
	switch (id)
	{
	case 1:
		if(!flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_7);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
		}
		break;
	case 2:
		if(!flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_10);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_10);
		}
		break;
	case 3:
		if(!flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_13);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_13);
		}
		break;
	default:
		break;
	}
}


//���õ�ǰλ��Ϊԭ��
void setOrigin(unsigned char id)
{
	if (id >= 1 && id <= 3)
	{
		motorPulse[0][id - MOTOR_START_NUM] = 5000;
		motorPulse[1][id - MOTOR_START_NUM] = 5000;
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
		if(flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_8);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_8);
		}
		break;
	case 2:
		if(flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_11);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_11);
		}
		break;
	case 3:
		if(flag)
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_14);
		}
		else
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		}
		break;
	default:
		break;
	}
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[3][id - MOTOR_START_NUM] = !flag;
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

//����ֹͣ
void emergencyStop()
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		setMotorSpeed(i + MOTOR_START_NUM, 0);
		setMotorPos_abs(i + MOTOR_START_NUM,getMotorPulse(i + MOTOR_START_NUM));
	}
}


//��ȡĳ������Ƿ��ƶ�����
u8 getMotorIsEnd(u8 id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorEnd[id - MOTOR_START_NUM];
	}
}


//��ȡȫ������Ƿ��ƶ�����
u8 getMotorsIsEnd()
{
	return motorEnd[0] && motorEnd[1] && motorEnd[1];
}


//����Ƿ񳬳�����
static void checkLImit(void)
{
	unsigned char i = 0;
	for (i = 0; i < MOTOR_NUM; i++)
	{
		//���ʵʱֵ�Ƿ񳬳�����
		if (motorPulse[0][i] < motorPulse[5][i])
		{
			setErrorState(MOVE_1_NegativeLimit * (i+ MOTOR_START_NUM));
		}
		else if (motorPulse[0][i] > motorPulse[4][i])
		{
			setErrorState(MOVE_1_PositiveLimit* (i + MOTOR_START_NUM));
		}
		//���Ŀ��ֵ�Ƿ񳬳�����
		if (motorPulse[1][i] < motorPulse[5][i])
		{
			motorPulse[1][i] = motorPulse[5][i];
			setErrorState(MOTOR_1_POS_NegativeLimit* (i + MOTOR_START_NUM));
		}
		else if (motorPulse[1][i] > motorPulse[4][i])
		{
			motorPulse[1][i] = motorPulse[4][i];
			setErrorState(MOTOR_1_POS_PositiveLimit* (i + MOTOR_START_NUM));
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
		if (!getMotorSpeed(i + MOTOR_START_NUM))
		{
			return;
		}
		
		if (motorPulse[0][i] != motorPulse[1][i])
		{
			//�˶�δ���
			LED_Pulse(200);
			motorEnd[i] = 0;
			speed = motorPulse[2][i];
			if (count[i] < speed)
			{
				switch (i + MOTOR_START_NUM)
				{
				case 1:
					GPIO_SetBits(GPIOE,GPIO_Pin_9);
					break;
				case 2:
					GPIO_SetBits(GPIOE,GPIO_Pin_12);
					break;
				case 3:
					GPIO_SetBits(GPIOE,GPIO_Pin_15);
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
					GPIO_ResetBits(GPIOE,GPIO_Pin_9);
					break;
				case 2:
					GPIO_ResetBits(GPIOE,GPIO_Pin_12);
					break;
				case 3:
					GPIO_ResetBits(GPIOE,GPIO_Pin_15);
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
		else
		{
			//�˶����
			motorEnd[i] = 1;		
		}
	}
}

//���Task
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


