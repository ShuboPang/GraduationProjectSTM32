#ifndef _PCA9685_H_
#define _PCA9685_H_
#include "sys.h"
#include "myiic.h"

#define PCA9685_adrr 0x80
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4


#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE


#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9


#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD


void PCA9685_write(unsigned char reg, unsigned char data);
u8 PCA9685_read(unsigned char reg);
u16 calculate_PWM(u8 angle);
void setPWMFreq(u8 freq);
void setPWM(u8 num, u16 on, u16 off);
void down();
void up();

#endif

