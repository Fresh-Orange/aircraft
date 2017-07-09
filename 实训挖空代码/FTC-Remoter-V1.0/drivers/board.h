
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f10x.h"
#include "string.h"
#include "FTC_Math.h"

#define ARMAPI extern "C"

/***************LED GPIO定义******************/
#define FTC_RCC_LED			RCC_APB2Periph_GPIOC
#define FTC_GPIO_LED		GPIOC
#define FTC_Pin_LED			GPIO_Pin_13
/*********************************************/

/***************I2C GPIO定义******************/
#define FTC_GPIO_I2C	GPIOA
#define I2C_Pin_SCL		GPIO_Pin_12
#define I2C_Pin_SDA		GPIO_Pin_11
#define FTC_RCC_I2C		RCC_APB2Periph_GPIOA
/*********************************************/

/***************SPI2 GPIO定义******************/
#define FTC_GPIO_SPI2		GPIOB
#define FTC_GPIO_CE2		GPIOB
#define SPI2_Pin_SCK		GPIO_Pin_13
#define SPI2_Pin_MISO		GPIO_Pin_14
#define SPI2_Pin_MOSI		GPIO_Pin_15
#define SPI2_Pin_CE2		GPIO_Pin_5
#define SPI2_Pin_CSN		GPIO_Pin_12
#define RCC_GPIO_SPI2		RCC_APB2Periph_GPIOB
#define RCC_GPIO_CE2		RCC_APB2Periph_GPIOB
/*********************************************/

/***************硬件中断优先级******************/
#define NVIC_UART_P	5
#define NVIC_UART_S	1
/***********************************************/

#include "FTC_Config.h"
#include "FTC_Drv_Usart1.h"
#include "FTC_Drv_ADC.h"
#include "FTC_Drv_LED.h"
#include "FTC_Drv_SPI2.h"
#include "FTC_Drv_Nrf24l01.h"
#include "FTC_Drv_EEPROM.h"
#include "FTC_Drv_I2C_soft.h"
#include "FTC_Drv_MPU6050.h"

void FTC_Remoter_board_Init(void);
void SysTick_IRQ(void);


#endif /* __BOARD_H__ */

// 




