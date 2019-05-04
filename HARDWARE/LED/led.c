#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//LED��������	   
//STM32F4����-�⺯���汾								  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);//GPIOF9,F10���øߣ�����

}

void LED_RunFlash(void)
{
	static u32 i = 0;
	if( i  >= 500 && i < 1000)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}
	else if(i >= 1000 )
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		i = 0;
	}
	i++;
}
