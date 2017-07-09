#ifndef __FTC_DRV_LED_H__
#define __FTC_DRV_LED_H__

#include "board.h"

class FTC_LED
{

public:
	
	static void Init(void);

	void ON1(void);
	void ON2(void);
	void OFF1(void);
	void OFF2(void);
};

extern FTC_LED led;

#endif

