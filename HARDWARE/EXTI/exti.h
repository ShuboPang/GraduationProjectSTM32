#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"  	
//////////////////////////////////////////////////////////////////////////////////	 
 
// STM32F407������
//�ⲿ�ж� ��������	   
//STM32F4����-�⺯���汾					  
////////////////////////////////////////////////////////////////////////////////// 	 


void Dis_Init(void);
void EXTIX_Init(void);	//�ⲿ�жϳ�ʼ��	
void stopDisRun(void);
void Timer_Config(void);
u8 IsDisRunning(void);
void GetDistanceDelay(void);
#endif
