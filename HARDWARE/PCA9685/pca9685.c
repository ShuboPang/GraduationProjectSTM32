#include "pca9685.h"
#include "delay.h"
#include <math.h>

void PCA9685_write(unsigned char reg, unsigned char data)
{
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

u8 PCA9685_read(unsigned char reg)
{
	u8 res;
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(PCA9685_adrr | 0X01);
	IIC_Wait_Ack();
	res = IIC_Read_Byte(0);
	IIC_Stop();
	return res;
}

void setPWMFreq(u8 freq) //
{
	u8 prescale, oldmode, newmode;
	double prescaleval;
	prescaleval = 25000000.0 / (4096 * freq*0.915);
	prescale = (u8)floor(prescaleval + 0.5) - 1;

	oldmode = PCA9685_read(PCA9685_MODE1);
	newmode = (oldmode & 0x7F) | 0x10; // sleep
	PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
	PCA9685_write(PCA9685_PRESCALE, prescale); // set the prescaler
	PCA9685_write(PCA9685_MODE1, oldmode);
	delay_ms(5);
	PCA9685_write(PCA9685_MODE1, oldmode | 0xa1);
}

void setPWM(u8 num, u16 on, u16 off)
{
	PCA9685_write(LED0_ON_L + 4 * num, on);
	PCA9685_write(LED0_ON_H + 4 * num, on >> 8);
	PCA9685_write(LED0_OFF_L + 4 * num, off);
	PCA9685_write(LED0_OFF_H + 4 * num, off >> 8);
}

u16 calculate_PWM(u8 angle) {
	return (int)(204.8*(0.5 + angle * 1.0 / 90));
}

void down() {
	u16 pwm = calculate_PWM(0);
	setPWM(0x0, 0, pwm);
	delay_ms(1);
	setPWM(0x1, 0, pwm);
	delay_ms(1);
	setPWM(0x2, 0, pwm);
	delay_ms(1);
	setPWM(0x3, 0, pwm);
	delay_ms(1);
	setPWM(0x4, 0, pwm);
	delay_ms(1);
	setPWM(0x5, 0, pwm);
	delay_ms(1);
	setPWM(0x6, 0, pwm);
	delay_ms(1);
	setPWM(0x7, 0, pwm);
}

void up() {
	u16 pwm = calculate_PWM(90);
	setPWM(0x0, 0, pwm);
	delay_ms(1);
	setPWM(0x1, 0, pwm);
	delay_ms(1);
	setPWM(0x2, 0, pwm);
	delay_ms(1);
	setPWM(0x3, 0, pwm);
	delay_ms(1);
	setPWM(0x4, 0, pwm);
	delay_ms(1);
	setPWM(0x5, 0, pwm);
	delay_ms(1);
	setPWM(0x6, 0, pwm);
	delay_ms(1);
	setPWM(0x7, 0, pwm);
}

