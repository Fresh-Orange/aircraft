/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_LED.cpp
 * 描述    ：LED
**********************************************************************************/
#include "FTC_Drv_LED.h"

FTC_LED led;
GPIO_InitTypeDef  GPIO_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure2;


void FTC_LED::Init(void)
{
	

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC  , ENABLE  );

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	
	GPIO_InitStructure2.GPIO_Pin  = GPIO_Pin_14;			  
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure2);
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);

}

void FTC_LED::ON1(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);	
}

void FTC_LED::ON2(void)
{		
	GPIO_SetBits(GPIOC, GPIO_Pin_14);		
}

void FTC_LED::OFF1(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void FTC_LED::OFF2(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/

