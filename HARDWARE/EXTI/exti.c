#include "exti.h"
#include "timer.h"
#include "delay.h"
#include "communication.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//�ⲿ�ж� ��������	   
//STM32F4����-�⺯���汾							  
////////////////////////////////////////////////////////////////////////////////// 

static u8 disState = 0;

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	resetCounter();//��ռ�����
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	

	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	 NVIC_InitTypeDef   NVIC_InitStructure;
	 EXTI_InitTypeDef   EXTI_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);//PB0 ���ӵ��ж���0


	  /* ����EXTI_Line0 */
	  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	  EXTI_Init(&EXTI_InitStructure);//����

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	  NVIC_Init(&NVIC_InitStructure);//����

}

void Dis_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOFʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//�ж��Ƿ��ڲ����
u8 IsDisRunning()
{
	return disState;
}

//�����������
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
			GPIO_SetBits(GPIOA, GPIO_Pin_4);//GPIOA4���øߣ�����������
			delay_us(15);
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			resetCounter();//��ռ�����
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












