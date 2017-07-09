/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Motor.cpp
 * 描述    ：电机控制相关函数
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;

void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw)
{
	//六轴X型
	motorPWM[0] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; //后右
	motorPWM[1] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; //前右
	motorPWM[2] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; //后左
	motorPWM[3] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; //前左
	motorPWM[4] = throttle - pidTermRoll - pidTermYaw;	//右
	motorPWM[5] = throttle + pidTermRoll + pidTermYaw;	//左
				
	for (u8 i = 0; i < 6; i++) 
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);

	//如果未解锁，则将电机输出设置为最低
	if(!ftc.f.ARMED || rc.rawData[THROTTLE] < 1200)	
		for(u8 i=0; i< 6 ; i++)
			motorPWM[i] = 1000;

	//写入电机PWM
	pwm.SetPwm(motorPWM);
	
}

void FTC_Motor::getPWM(uint16_t* pwm)
{
	*(pwm) = motorPWM[0];
	*(pwm+1) = motorPWM[1];
	*(pwm+2) = motorPWM[2];
	*(pwm+3) = motorPWM[3];
	*(pwm+4) = motorPWM[4];
	*(pwm+5) = motorPWM[5];	
}




/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
