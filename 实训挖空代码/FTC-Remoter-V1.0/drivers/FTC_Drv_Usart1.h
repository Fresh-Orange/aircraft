#ifndef _FTC_DRV_USART1_H_
#define _FTC_DRV_USART1_H_

#include "stm32f10x.h"

extern u8 Rx_Buf[2][32];	//两个32字节的串口接收缓存
extern u8 Rx_Ok0;		//接收完毕标志
extern u8 Rx_Ok1;

void Uart1_Init(u32 br_num);
void Uart1_IRQ(void);

void Uart1_Put_String(unsigned char *Str);
uint8_t Uart1_Put_Char(unsigned char DataToSend);
uint8_t Uart1_Put_Int16(uint16_t DataToSend);
uint8_t Uart1_Put_Float(float DataToSend);
void Usart1_Send_Buf(unsigned char *DataToSend , u8 data_num);

#endif

