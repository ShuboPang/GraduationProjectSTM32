#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "sys.h"
#include "usart.h"
#include "motor.h"

#define MAX_LENGTH 16
#define MAX_COUNT 5

static char s_buff[MAX_COUNT][MAX_LENGTH] = { 0 };

u8 checkSum(char *src, u32 length);
void setSendBuff();
void sendTo();




#endif // !_COMMUNICATION_H_
