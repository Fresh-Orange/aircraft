/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Drv_SPI2.cpp
 * ����    ��SPI2
**********************************************************************************/
#include "board.h"
#include "FTC_Drv_SPI2.h"


void FTC_SPI2::Init(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_GPIO_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_GPIO_CE2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	 
	/*���� SPI_NRF_SPI�� SCK,MISO,MOSI���� */ 
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_SCK| SPI2_Pin_MISO| SPI2_Pin_MOSI; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù��� 
	GPIO_Init(FTC_GPIO_SPI2, &GPIO_InitStructure);
	/*����SPI_NRF_SPI��CE���ţ���SPI_NRF_SPI�� CSN ����:*/
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_CE2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(FTC_GPIO_CE2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_CSN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(FTC_GPIO_SPI2, &GPIO_InitStructure);	
//	/*����SPI_NRF_SPI��IRQ���ţ�*/ 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //�������� 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(FTC_GPIO_SPI2, SPI2_Pin_CSN);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫�� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��ģʽ 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //���ݴ�С8λ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //ʱ�Ӽ��ԣ�����ʱΪ�� 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //��1��������Ч��������Ϊ����ʱ�� 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�ź���������� 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //4��Ƶ��9MHz 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI2, &SPI_InitStructure); 
	/* Enable SPI2 */ 
	SPI_Cmd(SPI2, ENABLE);
}

u8 FTC_SPI2::RW(u8 dat) 
{ 
	/* �� SPI���ͻ������ǿ�ʱ�ȴ� */ 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
	/* ͨ�� SPI2����һ�ֽ����� */ 
	SPI_I2S_SendData(SPI2, dat); 
	/* ��SPI���ջ�����Ϊ��ʱ�ȴ� */ 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
	/* Return the byte read from the SPI bus */ 
	return SPI_I2S_ReceiveData(SPI2); 
}


void FTC_SPI2::CE_H(void)
{
	GPIO_SetBits(FTC_GPIO_CE2, SPI2_Pin_CE2);
}

void FTC_SPI2::CE_L(void)
{
	GPIO_ResetBits(FTC_GPIO_CE2, SPI2_Pin_CE2);
}

void FTC_SPI2::CSN_H(void)
{
	GPIO_SetBits(FTC_GPIO_SPI2, SPI2_Pin_CSN);
}

void FTC_SPI2::CSN_L(void)
{
	GPIO_ResetBits(FTC_GPIO_SPI2, SPI2_Pin_CSN);
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
