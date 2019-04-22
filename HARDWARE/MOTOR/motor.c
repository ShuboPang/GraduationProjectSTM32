#include "motor.h"

/*
motorPulse[0] : 实时脉冲数 ；
motorPulse[1] : 目标脉冲数 ;
motorPulse[2] : 电机速度；
motorPulse[3] : 正极限 ;
motorPulse[4] : 正极限 ;
motorPulse[5] : 负极限；
*/
static u32 motorPulse[6][MOTOR_NUM] = { 0 };

//所有电机端口初始化
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

@function：设置电机转动方向
@para：
	id：电机号 范围：1-3
	falg：转动方向 0：正转；2：反转
@return：
	无
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


//获取电机当前脉冲数
u32 getMotorPulse(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[0][id - MOTOR_START_NUM];
	}
	return 0;
}

/*
设置电机绝对位置
@para：
	id：电机号 范围：1-3
	pulse：目标脉冲数 必须大于等于0
@return ：
	无

*/
void setMotorPos_abs(unsigned char id,u32 pulse)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		 motorPulse[1][id- MOTOR_START_NUM] =  pulse;
	}
}

/*
设置电机相对位置
@para：
	id：电机号 范围：1-3
	pulse：相对目标脉冲数 可正可负
@return ：
	无
*/
void setMotorPos_rela(unsigned char id,int pulse)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[1][id- MOTOR_START_NUM] = motorPulse[0][id- MOTOR_START_NUM] + pulse;
	}
}

//设置电机速度
void setMotorSpeed(unsigned char id, u16 speed)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[2][id- MOTOR_START_NUM] = speed;
	}
}

//设置正极限
void setPositiveLimit(unsigned char id, u32 limit)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[4][id - MOTOR_START_NUM] = limit;
	}
}

//设置负极限
void setNegativeLimit(unsigned char id, u32 limit)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		motorPulse[5][id - MOTOR_START_NUM] = limit;
	}
}

//检查是否超出极限
static void checkLImit(void)
{
	unsigned char i = 0;
	for (i = 0; i < MOTOR_NUM; i++)
	{
		//检查实时值是否超出脉冲
		if (motorPulse[0][i] < motorPulse[5][i] || motorPulse[0][i] > motorPulse[4][i])
		{
			//报警
		}
		//检查目标值是否超出脉冲
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

//发送脉冲 并实时更新每个轴的的实时脉冲数
//需放在定时器中断函数中定时执行
static void setMotorPulse(void)
{
	static u32 count[3] = {0};			//记录当前第几次进入函数
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
		//根据目标脉冲和当前脉冲判断电机转动方向
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


