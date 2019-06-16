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
#include "pca9685.h"
#include "myiic.h"

void sys_init()
{
	delay_init(168);
	uart_init(9600);
	Initial_UART2(9600);
	Initial_UART3(9600);
	Timer_Config();
	LED_Init();
	TIM3_Int_Init(2 - 1, 8400 - 1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�������Ϊ0.2ms 
	IIC_Init();
	PCA9685_write(PCA9685_MODE1, 0x0);
	setPWMFreq(50);
}

int main(void)
{
	sys_init();
	while(1)
	{
		sendTo(1);
	}
}
