#include "route.h"
#include "communication.h"
#include "error.h"
#include "motor.h"
#include "math.h"


static u32 g_speed = 0;			//�Զ�״̬ȫ���ٶ�

static u8 isComeBack = 0;	//�Ƿ񷵻�
static u8 isCycle = 0;		//�Ƿ�ѭ��
static u8 isSymmetry = 0;	//�Ƿ�Գ�
static u32 cycle_times = 0;	//ѭ������

static u8 isAuto = 0;//�Ƿ����Զ�ģʽ

static u32 calibration_heigh[CALIB_HEIGH_POINT][MOTOR_NUM] = { 0 };			//�궨�߶�5���������


/*********************  �����Զ�����ʱ�Ĳ���  ******************************/

//����ȫ���ٶ�
void setG_speed(u32 speed)
{
	g_speed = speed;
}

//��ȡȫ���ٶ�
u32 getG_speed()
{
	return g_speed;
}

//�����Ƿ񷵻�
void setComeBack(u8 state)
{
	isComeBack = state;
}

//��ȡ�Ƿ񷵻�
u8 getComeBack()
{
	return isComeBack;
}

//�����Ƿ�ѭ��
void setCycle(u8 state)
{
	isCycle = state;
}

//��ȡ�Ƿ�ѭ��
u8 getCycle()
{
	return isCycle;
}

//����ѭ������
void setCycleTimes(u32 times)
{
	cycle_times = times;
}

//��ȡѭ������
u8 getCycleTimes()
{
	return cycle_times;
}

//�����Ƿ�Գ�
void setSymmetrye(u8 state)
{
	isSymmetry = state;
}

//��ȡ�Ƿ�Գ�
u8 getSymmetry()
{
	return isSymmetry;
}

/***********************************************************/


//����Ƿ�궨�� 0:δ�궨���д�1���궨��
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


//���㷨��ֱ�߷���
u32 twoPoint_line(u32 x,u32 x1, u32 y1, u32 x2, u32 y2)
{
	return (y2 - y1) / (x2 - x1)*(x - x1) + y1;
}


//���ñ궨�������λ��ֵ
void set_calibration_heigh(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//�ٶȹ滮����
//ͨ���Ƚ�ÿ�����Ŀ�������뵱ǰ����ʹ������ͬʱ����ָ��λ��
void speed_Planning()
{
	u32 x_speed = getG_speed();
	setMotorSpeed(X_MOTOR, x_speed);		//��ȫ���ٶ����ø�x��
	u32 y_speed = 0;
	y_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)));
	setMotorSpeed(Y_MOTOR, y_speed);
	u32 z_speed = 0;
	z_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
	setMotorSpeed(Z_MOTOR, z_speed);
}


//�̶��߶�
void fixedHeight(u32 distance)
{
	u32 ret = 0;
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (distance < 5 * DISTANCE_PRECISION)
		{
			ret = 0;
		}
		else if (distance >= 5 * DISTANCE_PRECISION&&distance <= 10 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 5 * DISTANCE_PRECISION, calibration_heigh[0][i], 10 * DISTANCE_PRECISION, calibration_heigh[1][i]);
		}
		else if (distance >= 10 * DISTANCE_PRECISION&&distance <= 15 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 10 * DISTANCE_PRECISION,  calibration_heigh[1][i], 15 * DISTANCE_PRECISION, calibration_heigh[2][i]);
		}
		else if (distance >= 15 * DISTANCE_PRECISION&&distance <= 20 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 15 * DISTANCE_PRECISION,  calibration_heigh[2][i], 20 * DISTANCE_PRECISION, calibration_heigh[3][i]);
		}
		else if (distance >= 20 * DISTANCE_PRECISION&&distance <= 25 * DISTANCE_PRECISION)
		{
			ret = twoPoint_line(distance, 20 * DISTANCE_PRECISION,  calibration_heigh[3][i], 25 * DISTANCE_PRECISION, calibration_heigh[4][i]);
		}
		else
		{
			ret = 0;
		}
		setMotorPos_abs(i + MOTOR_START_NUM, ret);
	}
	isAuto = 1;
	speed_Planning();
}
