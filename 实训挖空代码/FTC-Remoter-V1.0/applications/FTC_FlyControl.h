#ifndef __FTC_FLYCONTROL_H
#define __FTC_FLYCONTROL_H

#include "FTC_Config.h"

#define FLYANGLE_MAX 250  //最大飞行倾角25度

enum {
    PIDROLL,
    PIDPITCH,
    PIDYAW,
		PIDITEMS
};

class FTC_FlyControl
{

public:
	
	FTC_PID pid[PIDITEMS];

	FTC_FlyControl();

	//姿态控制
	void Attitude_Loop(void);
	
private:
	
	uint8_t yawRate;
	int32_t RateError[3];
	void PID_Reset(void);
};

extern FTC_FlyControl fc;

#endif























