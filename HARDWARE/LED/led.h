#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//LED��������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PAout(6)	// DS0
#define LED1 PAout(7)	// DS1	 

void LED_Init(void);//��ʼ��
void LED_RunFlash(void); //run led flash
#endif
