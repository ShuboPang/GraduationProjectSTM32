#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//LED驱动代码	   
//STM32F4工程-库函数版本								  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PA6和PA7为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);//GPIOA6,A7设置高，灯灭

}

void LED_RunFlash(u32 time)
{
	static u32 i = 0;
	if( i  >= time && i < time*2)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}
	else if(i >= time*2)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		i = 0;
	}
	i++;
}


void LED_Pulse(u32 time)
{
	static u32 i = 0;
	if (i >= time && i < time * 2)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}
	else if (i >= time * 2)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		i = 0;
	}
	i++;
}