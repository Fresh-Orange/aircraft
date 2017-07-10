/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��board.c
 * ����    ��Ӳ����ʼ��
**********************************************************************************/
#include "board.h"

// cycles per microsecond
static volatile uint32_t usTicks = 0;
// �δ�ʱ���������� ,49������
static volatile uint32_t sysTickUptime = 0;

static void cycleCounterInit(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    usTicks = clocks.SYSCLK_Frequency / 1000000;
}

void SysTick_IRQ(void)
{
	sysTickUptime++;
	scheduler.cnt_1ms++;
	scheduler.cnt_2ms++;
	scheduler.cnt_5ms++;
	scheduler.cnt_10ms++;
	scheduler.cnt_20ms++;
}

uint32_t GetSysTime_us(void) 
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickUptime);
    return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void DelayUs(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void DelayMs(uint32_t ms)
{
    while (ms--)
        DelayUs(1000);
}

void FTC_Hexacopter_board_Init(void)
{
	//�ж����ȼ��������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//��ʼ��ϵͳ�δ�ʱ��
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);	
	
	//��ʼ��ģ��I2C
	FTC_I2C_Soft::Init();
	//��ʼ��SPI2
	FTC_SPI2::Init();
	//��ʼ��NRF
	nrf.Init(MODEL_TX2,251);
	
	//��ʼ����ʱ�����PWM,24KHz
	FTC_PWM::out_Init(24000);

	uart3.Init(9600);
	
	FTC_LED::Init();	
	//��ȡ��ѹADC��ʼ��
	FTC_ADC_Init();
	
	//����flash
	FLASH_Unlock();	
	//��ʼ������eeprom����
	EE_Init();	
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
