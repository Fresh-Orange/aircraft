/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_FlyControl.cpp
 * ����    �����п���
**********************************************************************************/
#include "FTC_FlyControl.h"

FTC_FlyControl fc;

FTC_FlyControl::FTC_FlyControl()
{
	rollPitchRate = 150;
	yawRate = 50;
	
	altHoldDeadband = 100;
	
	//����PID����
	PID_Reset();
}

//����PID����
void FTC_FlyControl::PID_Reset(void)
{
	pid[PIDROLL].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDPITCH].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDYAW].set_pid(0.8, 0.45, 0, 200);
	pid[PIDANGLE].set_pid(5, 0, 0, 0);
	pid[PIDMAG].set_pid(2, 0, 0, 0);
 	pid[PIDVELZ].set_pid(1.5, 0.5, 0.002, 150);
 	pid[PIDALT].set_pid(1.2, 0, 0, 200);
}

//��������̬�⻷����
void FTC_FlyControl::Attitude_Outter_Loop(void)
{
	//motor.writeMotor(0,0,0,0);
}

//��������̬�ڻ�����
void FTC_FlyControl::Attitude_Inner_Loop(void)
{
	//to do
}	

//�������߶��⻷����
void FTC_FlyControl::Altitude_Outter_Loop(void)
{
	//to do
}

//�������߶��ڻ�����
void FTC_FlyControl::Altitude_Inner_Loop(void)
{
	//to do
}

void FTC_FlyControl::AltHoldReset(void)
{
	AltHold = nav.position.z;
}

/************************ (C) COPYRIGHT 2015 FTC *****END OF FILE**********************/
