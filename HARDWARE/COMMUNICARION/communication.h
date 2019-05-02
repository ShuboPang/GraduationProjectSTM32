#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "sys.h"
#include "usart.h"
#include "motor.h"

#define MAX_LENGTH 8
#define MAX_COUNT 5

static u32 s_buff[MAX_COUNT][MAX_LENGTH] = { 0 };

u32 checkSum(char *src);
void setSendBuff();
void sendTo();




#endif // !_COMMUNICATION_H_
