#include "sys.h"
#include "string.h"
#include "communication.h"
#include "UART3.h"

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 
static unsigned char ucRxCnt = 0;

void Initial_UART3(unsigned long baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
		//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA2����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA3����ΪUSART3
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);    
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_ClearFlag(USART3,USART_FLAG_TC);	
	USART_Cmd(USART3, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{	   
		 USART_SendData(USART3, TxBuffer[TxCounter++]); 
		USART_ClearITPendingBit(USART3, USART_IT_TXE);
		if(TxCounter == count) USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	 }
	 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		CopeSerial3Data((unsigned char)USART3->DR);//��������
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	
	USART_ClearITPendingBit(USART3,USART_IT_ORE);
}


void UART3_Put_Char(unsigned char DataToSend)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		//�ȴ��ϴη��ͽ���
	USART_SendData(USART3, (unsigned char)DataToSend);				//�������ݵ�����
}

void UART3_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')UART3_Put_Char(0x0d);
			else if(*Str=='\n')UART3_Put_Char(0x0a);
				else UART3_Put_Char(*Str);
		Str++;
	}
}

void CopeSerial3Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	ucRxBuffer[ucRxCnt++] = ucData;

	if (ucRxCnt == 2)
	{
		setDistance(ucRxBuffer[0]*256+ ucRxBuffer[1]);
		ucRxCnt = 0;
	}
	
}