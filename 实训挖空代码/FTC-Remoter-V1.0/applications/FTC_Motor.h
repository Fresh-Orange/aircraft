#ifndef __FTC_MOTOR_H
#define __FTC_MOTOR_H

#include "FTC_Config.h"

#define MINTHROTTLE 1100
#define MAXTHROTTLE 1900

class FTC_Motor
{

public:
	
	void writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw);
	
	void getPWM(uint16_t* pwm);

private:
	
	uint16_t motorPWM[6];	
	
};

extern FTC_Motor motor;

#endif





