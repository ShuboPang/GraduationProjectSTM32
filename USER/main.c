#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "motor.h"
#include "timer.h"
#include "communication.h"
#include "UART2.h"
#include "UART3.h"
#include "exti.h"

void sys_init()
{
	delay_init(168);
	uart_init(115200);
	Initial_UART2(9600);
	Initial_UART3(9600);
	motor_pos_init();
	Timer_Config();
	Dis_Init();
	EXTIX_Init();
	LED_Init();
	motors_init();
	TIM3_Int_Init(2 - 1, 8400 - 1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�������Ϊ0.2ms 
	//����������
	setPositiveLimit(1, 10000);
	setPositiveLimit(2, 10000);
	setPositiveLimit(3, 10000);
	emergencyStop();		//��ֹ�ϵ�Ϳ�ʼ�˶�
}

int main(void)
{
	sys_init();
	while(1)
	{
		GetDistanceDelay();
	}
}
