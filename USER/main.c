#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "motor.h"

int main(void)
{
	u32 t=0;
	delay_init(168);
	uart_init(115200);
	LED_Init();
	motors_init();
	
  while(1){
    printf("b");
		delay_ms(500);
		LED_RunFlash();
	}
}
