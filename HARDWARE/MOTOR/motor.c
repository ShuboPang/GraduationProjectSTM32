#include "motor.h"
#include "error.h"
#include "led.h"

/*
motorPulse[0] : 实时脉冲数 ；
motorPulse[1] : 目标脉冲数 ;
motorPulse[2] : 电机速度；
motorPulse[3] : 实时转动方向 ;
motorPulse[4] : 正极限 ;
motorPulse[5] : 负极限；
motorPulse[6] : 电机正转方向；
*/
static u32 motorPulse[7][MOTOR_NUM] = { 0 };


//运动结束标志位
static u8 motorEnd[MOTOR_NUM] = { 0 };

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
	  for (int i = 1; i <= 3; i++)
	  {
		  motorSetEn(i, 1);
	  }
}


//初始化电机速度和位置参数
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


//电机使能
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


//设置当前位置为原点
void setOrigin(unsigned char id)
{
	if (id >= 1 && id <= 3)
	{
		motorPulse[0][id - MOTOR_START_NUM] = 5000;
		motorPulse[1][id - MOTOR_START_NUM] = 5000;
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


//获取电机当前脉冲数
u32 getMotorPulse(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[0][id - MOTOR_START_NUM];
	}
	return 0;
}

//获取电机目标脉冲数
u32 getMotorEndPulse(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[1][id - MOTOR_START_NUM];
	}
	return 0;
}

//获取电机速度
u32 getMotorSpeed(unsigned char id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorPulse[2][id - MOTOR_START_NUM];
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

//紧急停止
void emergencyStop()
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		setMotorSpeed(i + MOTOR_START_NUM, 0);
		setMotorPos_abs(i + MOTOR_START_NUM,getMotorPulse(i + MOTOR_START_NUM));
	}
}


//获取某个电机是否移动结束
u8 getMotorIsEnd(u8 id)
{
	if (id >= MOTOR_START_NUM && id <= MOTOR_NUM)
	{
		return motorEnd[id - MOTOR_START_NUM];
	}
}


//获取全部电机是否移动结束
u8 getMotorsIsEnd()
{
	return motorEnd[0] && motorEnd[1] && motorEnd[1];
}


//检查是否超出极限
static void checkLImit(void)
{
	unsigned char i = 0;
	for (i = 0; i < MOTOR_NUM; i++)
	{
		//检查实时值是否超出极限
		if (motorPulse[0][i] < motorPulse[5][i])
		{
			setErrorState(MOVE_1_NegativeLimit * (i+ MOTOR_START_NUM));
		}
		else if (motorPulse[0][i] > motorPulse[4][i])
		{
			setErrorState(MOVE_1_PositiveLimit* (i + MOTOR_START_NUM));
		}
		//检查目标值是否超出极限
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

//发送脉冲 并实时更新每个轴的的实时脉冲数
//需放在定时器中断函数中定时执行
static void setMotorPulse(void)
{
	static u32 count[3] = {0};			//记录当前第几次进入函数
	u32 speed = 0;
	for (u32 i = 0; i < MOTOR_NUM; i++)
	{
		if (!getMotorSpeed(i + MOTOR_START_NUM))
		{
			return;
		}
		
		if (motorPulse[0][i] != motorPulse[1][i])
		{
			//运动未完成
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
			//运动完成
			motorEnd[i] = 1;		
		}
	}
}

//电机Task
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


