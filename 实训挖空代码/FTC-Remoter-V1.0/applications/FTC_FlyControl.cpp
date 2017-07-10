/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_FlyControl.cpp
 * ����    �����п���
**********************************************************************************/
#include "FTC_FlyControl.h"

FTC_FlyControl fc;


FTC_FlyControl::FTC_FlyControl()
{
	yawRate = 20;
	//����PID����
	PID_Reset();
}

//����PID����
void FTC_FlyControl::PID_Reset(void)
{
	pid[PIDROLL].set_pid(50, 15, 65, 2000000);
	pid[PIDPITCH].set_pid(50, 15, 65, 2000000);
	pid[PIDYAW].set_pid(30, 0, 40, 2000000);
}

//��������̬����
void FTC_FlyControl::Attitude_Loop(void)
{
	int32_t PIDTerm[3];
	int32_t	errorAngle[3];
	
	//����Ƕ����ֵ
	errorAngle[ROLL] = constrain_int32((rc.Command[ROLL] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.x * 10; 
	errorAngle[PITCH] = constrain_int32((rc.Command[PITCH] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu.angle.y * 10; 
	errorAngle[YAW] = (imu.magHold - imu.angle.z) * 10;
	
	for(u8 i=0; i<3;i++)
	{
		//�����ŵ��ڼ��ֵʱ��������
		if ((rc.rawData[THROTTLE]) < RC_MINCHECK)	
			pid[i].reset_I();
		
		//�õ�PID���
		PIDTerm[i] = pid[i].get_pid(errorAngle[i], PID_LOOP_TIME);
	}
	
	//PID���תΪ���������
	motor.writeMotor(rc.Command[THROTTLE], PIDTerm[ROLL], PIDTerm[PITCH], PIDTerm[YAW]);
}

/************************ (C) COPYRIGHT 2015 FTC *****END OF FILE**********************/
