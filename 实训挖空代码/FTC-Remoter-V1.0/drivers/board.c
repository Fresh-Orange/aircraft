/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：board.c
 * 描述    ：硬件初始化
**********************************************************************************/
#include "board.h"

void SysTick_IRQ(void)
{
	scheduler.cnt_1ms++;
	scheduler.cnt_2ms++;
	scheduler.cnt_5ms++;
	scheduler.cnt_10ms++;
	scheduler.cnt_30ms++;
}

void FTC_Remoter_board_Init(void)
{
	//中断优先级组别设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//初始化系统滴答定时器
	SysTick_Config(SystemCoreClock / 1000);	
	
	//初始化模拟I2C
	FTC_I2C_Soft::Init();	
	
	//初始化串口1
	Uart1_Init(500000);
	
	//初始化SPI2
	FTC_SPI2::Init();
	//初始化NRF
	nrf.Init(MODEL_RX2,251);
	
	if(nrf.Check())
		scheduler.cnt_30ms++;
	
	//初始化ADC采样
	ADC1_Init();
	
	FTC_LED::Init();	
	
	//解锁flash
	FLASH_Unlock();	
	//初始化虚拟eeprom设置
	EE_Init();	
	
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
