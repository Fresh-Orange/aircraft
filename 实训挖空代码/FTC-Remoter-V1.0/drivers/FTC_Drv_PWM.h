#ifndef __FTC_DRV_PWM_H__
#define __FTC_DRV_PWM_H__

#include "board.h"

#define MAXMOTORS 6

class FTC_PWM
{

public:
	static void out_Init(uint16_t hz);
	void SetPwm(uint16_t pwm[MAXMOTORS]);
	
};

extern FTC_PWM pwm;

#endif

