/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Motor.cpp
 * ����    �����������غ���
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;

void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw)
{
	//����X��
	motorPWM[0] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; //����
	motorPWM[1] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; //ǰ��
	motorPWM[2] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; //����
	motorPWM[3] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; //ǰ��
	motorPWM[4] = throttle - pidTermRoll - pidTermYaw;	//��
	motorPWM[5] = throttle + pidTermRoll + pidTermYaw;	//��
				
	for (u8 i = 0; i < 6; i++) 
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);

	//���δ�������򽫵���������Ϊ���
	if(!ftc.f.ARMED || rc.rawData[THROTTLE] < 1200)	
		for(u8 i=0; i< 6 ; i++)
			motorPWM[i] = 1000;

	//д����PWM
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
