#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//LED��������	   
//STM32F4����-�⺯���汾								  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PA6��PA7Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);//GPIOA6,A7���øߣ�����

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