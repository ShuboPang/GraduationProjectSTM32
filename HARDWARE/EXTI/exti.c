#include "exti.h"
#include "timer.h"
#include "delay.h"
#include "communication.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//外部中断 驱动代码	   
//STM32F4工程-库函数版本							  
////////////////////////////////////////////////////////////////////////////////// 

static u8 disState = 0;

static u8 echoState[2] = { 0 };

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	resetCounter();//清空计数器
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
}	

	   
//外部中断初始化程序
//初始化PA0为中断输入.
void EXTIX_Init(void)
{
	 NVIC_InitTypeDef   NVIC_InitStructure;
	 EXTI_InitTypeDef   EXTI_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);//PB0 连接到中断线0


	  /* 配置EXTI_Line0 */
	  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	  EXTI_Init(&EXTI_InitStructure);//配置

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	  NVIC_Init(&NVIC_InitStructure);//配置

}

void Dis_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void Timer_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructer.TIM_Period = 65535;//定时周期  这里没开启定时中断，这个周期最大到2米距离
	TIM_TimeBaseInitStructer.TIM_Prescaler = 840 - 1; //  分频系数
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	/*定时器初始化完成*/
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructer);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_Cmd(TIM2, DISABLE);//¹关闭定时器使能
}



//超声波控制IO
void setTrig(u8 state)
{
	if (state)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	}
}


u8 getEcho()
{
	return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
}


//判断是否在测距中
u8 IsDisRunning()
{
	return disState;
}

//结束测距运行
void stopDisRun()
{
	disState = 0;
}

void GetDistanceDelay(void)
{
	disState = 1;
	u32 distance_cm=0;
	setTrig(1);
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);//使能TIM2定时器
	while(TIM_GetCounter(TIM2) < 11);
	setTrig(0);
	TIM_SetCounter(TIM2, 0);
	
	while(getEcho() == 0)
	{
		if(TIM_GetCounter(TIM2) > 60000)
		{
			disState = 0;
			distance_cm = 0;
			TIM_Cmd(TIM2, DISABLE);
			return;
		}
	}
	TIM_SetCounter(TIM2, 0);
	while(getEcho() == 1)
	{
		int count = TIM_GetCounter(TIM2);
		if(count > 60000)
		{
			disState = 0;
			distance_cm = 0;
			TIM_Cmd(TIM2, DISABLE);
			return;
		}
	}
	u32 dis_count = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2, DISABLE);
	distance_cm = (unsigned int)(((long)(dis_count) * 1.7));
	setDistance(distance_cm);
	disState = 0;
}
