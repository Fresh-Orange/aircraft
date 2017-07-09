/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_LED.cpp
 * 描述    ：LED
**********************************************************************************/
#include "FTC_Drv_LED.h"

FTC_LED led;

void FTC_LED::Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(FTC_RCC_LED,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = FTC_Pin_LED;
	GPIO_Init(FTC_GPIO_LED, &GPIO_InitStructure);
}

void FTC_LED::ON(void)
{
	GPIO_ResetBits(FTC_GPIO_LED, FTC_Pin_LED);		
}

void FTC_LED::OFF(void)
{
	GPIO_SetBits(FTC_GPIO_LED, FTC_Pin_LED);	
}




/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/

