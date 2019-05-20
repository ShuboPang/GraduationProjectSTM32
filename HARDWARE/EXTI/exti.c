#include "exti.h"
#include "timer.h"
#include "delay.h"
#include "communication.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//外部中断 驱动代码	   
//STM32F4工程-库函数版本							  
////////////////////////////////////////////////////////////////////////////////// 

static u8 disState = 0;

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
	
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PB0 连接到中断线0


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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
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

void distanceStart()
{
	static u32 i = 0;
	if (!IsDisRunning())
	{
		if (i < 100)
		{
			i++;
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_4);//GPIOA4设置高，超声波发射
			delay_us(15);
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			resetCounter();//清空计数器
			disState = 1;
			i = 0;
		}
	}
	else
	{
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
		{
			u32 times = getCounterTime();
			u32 distance = times * 0.17;       //us  *  speed(mm/us)
			setDistance(distance);
			stopDisRun();
		}
	}
}












