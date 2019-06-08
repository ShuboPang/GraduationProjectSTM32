#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "sys.h"
#include "motor.h"

#define CALIB_HEIGH_POINT 5			//�궨�߶���Ҫ�ĵ���
#define CALIB_ANGLE_POINT 6			//�궨�Ƕ���Ҫ�ĵ���
#define DISTANCE_PRECISION 1        //������С����

/************************ �����Զ����еĲ���  *************************/
void setG_speed(u32 speed);

u32 getG_speed(void);

//�����Ƿ񷵻�
void setComeBack(u8 state);

//��ȡ�Ƿ񷵻�
u8 getComeBack(void);

//�����Ƿ�ѭ��
void setCycle(u8 state);

//��ȡ�Ƿ�ѭ��
u8 getCycle(void);

//����ѭ������
void setCycleTimes(u32 times);

//��ȡѭ������
u8 getCycleTimes(void);

//�����Ƿ�Գ�
void setSymmetrye(u8 state);

//��ȡ�Ƿ�Գ�
u8 getSymmetry(void);

/***************************************************/


u8 checkIsCalib(void);		//����Ƿ�궨��

void set_calibration_heigh(u8 point);

u32 twoPoint_line(u32 x, u32 x1, u32 y1, u32 x2, u32 y2);
/*****************************************************/

//���滮���Ŀ�����������˶�
void route_setPulse();

//��·���滮���������λ��
void route_add(u32 *pulse);

//������Ҫ�˶���λ������
u32 get_route_count();

//����·������λ��
void reset_route();

//���õ������Ŀ��λ��
void set_route_one_pulse(u8 i, u8 id, u32 pulse);

//��ʼ�Զ�ģʽ
void startAutoMode();

//ֹͣ�Զ�ģʽ  �Ὣ��ǰ�������е�λ��������ɺ��������������ֹͣ
void endAutoMode();

void speed_Planning(void);		//�ٶȹ滮����
void fixedHeight(u32 distance);	//�̶��߶�
void fixedAngle(u32 angle);		//�̶��Ƕ�
void fixedLength(u32 data);		//�̶�����
								
void circle(u32 radius);			//��Բ
#endif // !_ROUTE_H_
