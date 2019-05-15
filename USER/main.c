#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "motor.h"
#include "timer.h"
#include "communication.h"
#include "UART2.h"

void sys_init()
{
	delay_init(168);
	uart_init(115200);
	Initial_UART2(9600);
	LED_Init();
	motors_init();
	TIM3_Int_Init(10 - 1, 8400 - 1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10��Ϊ1ms 
	//����������
	setPositiveLimit(1, 10000);
	setPositiveLimit(2, 10000);
	setPositiveLimit(3, 10000);
}

int main(void)
{
	sys_init();
	while(1)
	{
		delay_ms(1);
		sendTo();
	}
}
