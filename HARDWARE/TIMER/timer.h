#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//定时器 驱动代码	   
//STM32F4工程-库函数版本							  
////////////////////////////////////////////////////////////////////////////////// 	


void TIM3_Int_Init(u16 arr,u16 psc);
void resetCounter();
u32  getCounterTime(void);
void Tim3_taskGo();
#endif
