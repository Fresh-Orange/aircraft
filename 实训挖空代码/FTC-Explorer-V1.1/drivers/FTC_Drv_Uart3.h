#ifndef __FTC_DRV_UART3_H__
#define __FTC_DRV_UART3_H__

#include "board.h"

class FTC_UART3
{
public:
	
	u8 RxState3;

	static void Init(u32 br_num);
	void IRQ(void);

	void Put_Char(unsigned char DataToSend);
	void Put_String(unsigned char *Str);
	void Put_Buf(unsigned char *DataToSend , u8 data_num);
};

extern FTC_UART3 uart3;

#endif
