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
	TIM3_Int_Init(2 - 1, 8400 - 1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数次为0.2ms 
	//设置正极限
	setPositiveLimit(1, 10000);
	setPositiveLimit(2, 10000);
	setPositiveLimit(3, 10000);
	emergencyStop();		//防止上电就开始运动
}

int main(void)
{
	sys_init();
	while(1)
	{
		GetDistanceDelay();
	}
}
