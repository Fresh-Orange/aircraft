/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：board.c
 * 描述    ：硬件初始化
**********************************************************************************/
#include "board.h"

// cycles per microsecond
static volatile uint32_t usTicks = 0;
// 滴答定时器计数变量 ,49天后溢出
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
	//中断优先级组别设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//初始化系统滴答定时器
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);	
	
	//初始化模拟I2C
	FTC_I2C_Soft::Init();
	//初始化SPI2
	FTC_SPI2::Init();
	//初始化NRF
	nrf.Init(MODEL_TX2,251);
	
	//初始化定时器输出PWM,24KHz
	FTC_PWM::out_Init(24000);

	uart3.Init(9600);
	
	FTC_LED::Init();	
	//读取电压ADC初始化
	FTC_ADC_Init();
	
	//解锁flash
	FLASH_Unlock();	
	//初始化虚拟eeprom设置
	EE_Init();	
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
