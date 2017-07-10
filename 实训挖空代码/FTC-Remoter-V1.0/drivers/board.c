/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��board.c
 * ����    ��Ӳ����ʼ��
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
	//�ж����ȼ��������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//��ʼ��ϵͳ�δ�ʱ��
	SysTick_Config(SystemCoreClock / 1000);	
	
	//��ʼ��ģ��I2C
	FTC_I2C_Soft::Init();	
	
	//��ʼ������1
	Uart1_Init(500000);
	
	//��ʼ��SPI2
	FTC_SPI2::Init();
	//��ʼ��NRF
	nrf.Init(MODEL_RX2,251);
	
	if(nrf.Check())
		scheduler.cnt_30ms++;
	
	//��ʼ��ADC����
	ADC1_Init();
	
	FTC_LED::Init();	
	
	//����flash
	FLASH_Unlock();	
	//��ʼ������eeprom����
	EE_Init();	
	
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
