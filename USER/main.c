#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"

int main(void)
{
	u32 t=0;
	uart_init(115200);
	delay_init(84);
	LED_Init();
	
  while(1){
    printf("t:%d\r\n",t);
		delay_ms(500);
		LED_RunFlash();
		t++;
	}
}