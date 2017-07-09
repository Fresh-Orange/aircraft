#ifndef __FTC_DRV_SPI_H__
#define __FTC_DRV_SPI_H__

#include "stm32f10x.h"

class FTC_SPI2
{
	
public:
	
	static void Init(void);
	u8 RW(u8 dat);
	void CE_H(void);
	void CE_L(void);
	void CSN_H(void);
	void CSN_L(void);

};

#endif










