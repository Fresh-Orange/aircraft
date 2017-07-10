/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Scheduler.cpp
 * ����    ��������
**********************************************************************************/
#include "FTC_Scheduler.h"

FTC_Scheduler scheduler;

static void FTC_Loop_1000Hz(void)	//1msִ��һ��
{
	//����Ƿ��н��յ���������
	dt.Check_Event();
	
	//���ͷ���������
	dt.Data_Exchange();
}

static void FTC_Loop_500Hz(void)	//2msִ��һ��
{	
	//���´���������	
	imu.updateSensor();		
	
	//�����������̬
	imu.getAttitude();
	
	//��������̬�ڻ�����
	fc.Attitude_Inner_Loop();	
	
	//�������߶ȴ��������ݸ���
	nav.update();
}

static void FTC_Loop_200Hz(void)	//5msִ��һ��
{
	//��������̬�⻷����
	fc.Attitude_Outter_Loop();
}

static void FTC_Loop_100Hz(void)	//10msִ��һ��
{
	//��ص�ѹ����
	FTC_ADC_Battery();
}

static void FTC_Loop_50Hz(void)	//20msִ��һ��
{
	//ң��ͨ�����ݴ���
	rc.Cal_Command();
	
	//ҡ��λ�ü��
	rc.check_sticks();
	
	//����ͨ�����
	rc.CheckAUX();
	
	//ʧ�ر������
	dt.Failsafe_Check();
	
	//LEDָʾ�ƿ���
	ftc.Pilot_Light();
	
	//�������߶��⻷����
	fc.Altitude_Outter_Loop();	
	
	//�������߶��ڻ�����
	fc.Altitude_Inner_Loop();	
}

void FTC_Loop(void)
{
	if(scheduler.cnt_1ms >= 1){
		FTC_Loop_1000Hz();	
		scheduler.cnt_1ms = 0;
	}
	if(scheduler.cnt_2ms >= 2){
		FTC_Loop_500Hz();
		scheduler.cnt_2ms = 0;
	}		
	if(scheduler.cnt_5ms >= 5){	
		FTC_Loop_200Hz();
		scheduler.cnt_5ms = 0;
	}
	if(scheduler.cnt_10ms >= 10){		
		FTC_Loop_100Hz();
		scheduler.cnt_10ms = 0;
	}
	if(scheduler.cnt_20ms >= 20){		
		FTC_Loop_50Hz();
		scheduler.cnt_20ms = 0;
	}
}

FTC_Scheduler::FTC_Scheduler()
{
	cnt_1ms = cnt_2ms = cnt_5ms = cnt_10ms = cnt_20ms	= 0;
}
/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
