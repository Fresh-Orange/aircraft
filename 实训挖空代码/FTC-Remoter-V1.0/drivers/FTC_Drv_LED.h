#ifndef __FTC_DRV_LED_H__
#define __FTC_DRV_LED_H__

#include "board.h"

class FTC_LED
{

public:
	
	static void Init(void);

	void ON(void);
	void OFF(void);

};

extern FTC_LED led;

#endif

