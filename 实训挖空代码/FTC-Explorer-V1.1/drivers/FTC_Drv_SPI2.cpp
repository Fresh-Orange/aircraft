/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_SPI2.cpp
 * 描述    ：SPI2
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
	 
	/*配置 SPI_NRF_SPI的 SCK,MISO,MOSI引脚 */ 
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_SCK| SPI2_Pin_MISO| SPI2_Pin_MOSI; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能 
	GPIO_Init(FTC_GPIO_SPI2, &GPIO_InitStructure);
	/*配置SPI_NRF_SPI的CE引脚，和SPI_NRF_SPI的 CSN 引脚:*/
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_CE2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(FTC_GPIO_CE2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI2_Pin_CSN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(FTC_GPIO_SPI2, &GPIO_InitStructure);	
//	/*配置SPI_NRF_SPI的IRQ引脚，*/ 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //上拉输入 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(FTC_GPIO_SPI2, SPI2_Pin_CSN);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主模式 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //数据大小8位 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //时钟极性，空闲时为低 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //第1个边沿有效，上升沿为采样时刻 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS信号由软件产生 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //4分频，9MHz 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI2, &SPI_InitStructure); 
	/* Enable SPI2 */ 
	SPI_Cmd(SPI2, ENABLE);
}

u8 FTC_SPI2::RW(u8 dat) 
{ 
	/* 当 SPI发送缓冲器非空时等待 */ 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
	/* 通过 SPI2发送一字节数据 */ 
	SPI_I2S_SendData(SPI2, dat); 
	/* 当SPI接收缓冲器为空时等待 */ 
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
