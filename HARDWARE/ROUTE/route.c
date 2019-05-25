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

static u32 calibration_angle[CALIB_ANGLE_POINT][MOTOR_NUM] = { 0 };			//�궨�Ƕ�6���������

#define MAX_COUNT 256

//��Ҫ������Ŀ��λ��
static struct route_pulse
{
	u8 count;
	u32 pulse[3][MAX_COUNT];
}Route_pulse;

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

//��ʼ�Զ�ģʽ
void startAutoMode()
{
	isAuto = 1;
}

//ֹͣ�Զ�ģʽ  �Ὣ��ǰ�������е�λ��������ɺ��������������ֹͣ
void endAutoMode()
{
	isAuto = 0;
}

/***********************************************************/

//���õ������Ŀ��λ��
void set_route_one_pulse(u8 i,u8 id, u32 pulse)
{
	if (i >= MOTOR_START_NUM && i <= MOTOR_NUM)
	{
		Route_pulse.pulse[id - MOTOR_START_NUM][i + MOTOR_START_NUM] = pulse;
	}
}

//����·������λ��
void reset_route()
{
	Route_pulse.count = 0;			//����������㣬�ͻ����¸���λ��
}

//��·���滮���������λ��
void route_add(u32 *pulse)
{
	Route_pulse.count++;
	Route_pulse.pulse[0][Route_pulse.count - 1] = pulse[0];
	Route_pulse.pulse[1][Route_pulse.count - 1] = pulse[1];
	Route_pulse.pulse[2][Route_pulse.count - 1] = pulse[2];
}

//���滮���Ŀ�����������˶�
void route_setPulse()
{
	static u32 i = 0;
	static u32 count = 0;		//ѭ������
	if (isAuto)
	{
		if (getMotorsIsEnd()&&(i != Route_pulse.count))
		{
			for (u8 j = 0; j < MOTOR_NUM; j++)
			{
				setMotorPos_abs(j + MOTOR_START_NUM, Route_pulse.pulse[j][i]);
			}
			speed_Planning();
			i++;
		}
		//�켣λ�÷������
		else if (i >= Route_pulse.count)
		{
			//�ж��˶��Ƿ����������֮��ſ��Է�����һ�����λ��
			if (getMotorsIsEnd())
			{
				//�ж��Ƿ���ѭ��
				if (getCycle())
				{
					//�ж��Ƿ�Գ��˶�
					if (getSymmetry())
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							u32 cur_pulse = getMotorPulse(j + MOTOR_START_NUM);
							setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]*2 - cur_pulse);
						}
						speed_Planning();
					}
					else
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]);
						}
						speed_Planning();
					}

					if (count >= getCycleTimes())
					{
						isAuto = 0;
						count = 0;
					}
					else
					{
						count++;
					}

				}
				else
				{
					//�����Զ�����
					isAuto = 0;
					count = 0;
				}
				i = 0;

				//�Զ����й켣����
				if (isAuto == 0)
				{
					
					if (getComeBack())
					{
						for (u8 j = 0; j < MOTOR_NUM; j++)
						{
							if (getMotorPulse(j + MOTOR_START_NUM) != calibration_heigh[1][j])
							{
								setMotorPos_abs(j + MOTOR_START_NUM, calibration_heigh[1][j]);
							}
							speed_Planning();
						}
					}
				}
			}
		}
	}
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


//���ø߶ȱ궨�������λ��ֵ
void set_calibration_heigh(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//���ýǶȱ궨�������λ��ֵ
void set_calibration_angle(u8 point)
{
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		calibration_heigh[point][i] = getMotorPulse(i + 1);
	}
}

//�ٶȹ滮����
//ͨ���Ƚ�ÿ�����Ŀ�������뵱ǰ����ʹ������ͬʱ����ָ��λ��
void speed_Planning(void)
{
	u32 x_speed = getG_speed();
	u32 y_speed = 0;
	u32 z_speed = 0;
	setMotorSpeed(X_MOTOR, x_speed);		//��ȫ���ٶ����ø�x��
	
	if (getMotorPulse(X_MOTOR) == getMotorEndPulse(X_MOTOR))
	{
		y_speed = getG_speed();
		if (getMotorPulse(Y_MOTOR) == getMotorEndPulse(Y_MOTOR))
		{
			z_speed = getG_speed();
		}
		else
		{
			z_speed = abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)))*y_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
		}
	}
	else
	{
		y_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Y_MOTOR) - getMotorEndPulse(Y_MOTOR)));
		z_speed = abs((int)(getMotorPulse(X_MOTOR) - getMotorEndPulse(X_MOTOR)))*x_speed / abs((int)(getMotorPulse(Z_MOTOR) - getMotorEndPulse(Z_MOTOR)));
	}
	if (x_speed < 10)
	{
		x_speed = 10;
	}
	if (y_speed < 10)
	{
		y_speed = 10;
	}
	if (z_speed < 10)
	{
		z_speed = 10;
	}
	setMotorSpeed(Y_MOTOR, y_speed);
	setMotorSpeed(Z_MOTOR, z_speed);
}


//�̶��߶�
void fixedHeight(u32 distance)
{
	u32 ret = 0;
	u32 pulse_tmp[3] = { 0 };
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
		pulse_tmp[i] = ret;
//		setMotorPos_abs(i + MOTOR_START_NUM, ret);
	}
	route_add(pulse_tmp);
}

//�̶��Ƕ�
void fixedAngle(u32 angle)
{
	u32 pulse_tmp[3] = { 0 };
	/*
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (angle < 30 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 0, calibration_heigh[1][i], 5 * DISTANCE_PRECISION, calibration_angle[1][i]);
		}
		else if (angle >= 30 * DISTANCE_PRECISION&&angle <= 60 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 30 * DISTANCE_PRECISION, calibration_angle[1][i], 60 * DISTANCE_PRECISION, calibration_angle[3][i]);
		}
		else if (angle >= 60* DISTANCE_PRECISION&&angle <= 120 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 60 * DISTANCE_PRECISION, calibration_angle[3][i], 120 * DISTANCE_PRECISION, calibration_angle[4][i]);
		}
		else if (angle >= 120 * DISTANCE_PRECISION&&angle <= 150 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(angle, 120 * DISTANCE_PRECISION, calibration_angle[4][i], 150 * DISTANCE_PRECISION, calibration_angle[5][i]);
		}
		else
		{
			pulse_tmp[i] = getMotorPulse(i + MOTOR_START_NUM);
		}
	}
	*/
	pulse_tmp[0] = getMotorPulse(X_MOTOR);
	pulse_tmp[1] = -0.0644*((float)angle)*((float)angle)+16.22*((float)angle)-393+ calibration_heigh[1][1];
	pulse_tmp[2] = -0.081*((float)angle)*((float)angle) + 9.2965*((float)angle) +373 + calibration_heigh[1][1];

	route_add(pulse_tmp);
}

//�̶�����
void fixedLength(u32 data)
{
	u32 pulse_tmp[3] = { 0 };
	for (u8 i = 0; i < MOTOR_NUM; i++)
	{
		if (data < 5 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 0, calibration_heigh[1][i], 5 * DISTANCE_PRECISION, calibration_angle[0][i]);
		}
		else if (data >= 5 * DISTANCE_PRECISION&&data <= 10 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 5 * DISTANCE_PRECISION, calibration_angle[0][i], 10 * DISTANCE_PRECISION, calibration_angle[1][i]);
		}
		else if (data >= 10 * DISTANCE_PRECISION&&data <= 15 * DISTANCE_PRECISION)
		{
			pulse_tmp[i] = twoPoint_line(data, 10 * DISTANCE_PRECISION, calibration_angle[1][i], 15 * DISTANCE_PRECISION, calibration_angle[2][i]);
		}
		else
		{
			pulse_tmp[i] = getMotorPulse(i+ MOTOR_START_NUM);
		}
	}
	route_add(pulse_tmp);
}