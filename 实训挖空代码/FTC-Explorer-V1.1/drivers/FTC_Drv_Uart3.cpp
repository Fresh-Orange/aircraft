/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Drv_Uart.cpp
 * ����    ��LED
**********************************************************************************/
#include "FTC_Drv_Uart3.h"

FTC_UART3 uart3;

void FTC_UART3::Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	
	//Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	//Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART3ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ClockInit(USART3, &USART_ClockInitStruct);

	//ʹ��USART3�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//ʹ��USART3
	USART_Cmd(USART3, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
u8 TxBuffer3[256];
u8 TxCounter3=0;
u8 count3=0;
static u8 RxBuffer3[50];

void FTC_UART3::IRQ(void)
{
	if(USART3->SR & USART_SR_ORE)//ORE�ж�
	{
		u8 com_data = USART3->DR;
		//USART_ClearFlag(USART3,USART_IT_ORE);
	}
	if((USART3->SR & (1<<7))&&(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART3->DR = TxBuffer3[TxCounter3++]; //дDR����жϱ�־ 
		if(TxCounter3 == count3)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�ж�
		}
	}
	//�����ж� (���ռĴ����ǿ�) 
	if(USART3->SR & (1<<5))//if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    
	{
		static u8 ultraAltLow,ultraAltHigh;
		uint16_t ultraAltTemp;
		
		u8 com_data = USART3->DR;
		if(RxState3 == 0){
			ultraAltHigh = com_data;
			RxState3++;
		}
		else if(RxState3 == 1){
			ultraAltLow = com_data;
			ultraAltTemp = (uint16_t)(ultraAltHigh << 8)| ultraAltLow;
			
			if(ultraAltTemp > 0 && ultraAltTemp < 1500)
				nav.ultraAltRaw = ultraAltTemp;

			RxState3++;
		}
	}
}

void FTC_UART3::Put_Char(unsigned char DataToSend)
{
	TxBuffer3[count3++] = DataToSend;
  if(!(USART3->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
}
void FTC_UART3::Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str)
	{
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')Put_Char(0x0d);
		else if(*Str=='\n')Put_Char(0x0a);
			else Put_Char(*Str);
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}
void FTC_UART3::Put_Buf(unsigned char *DataToSend , u8 data_num)
{
	for(u8 i=0;i<data_num;i++)
		TxBuffer3[count3++] = *(DataToSend+i);
	if(!(USART3->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  
}


