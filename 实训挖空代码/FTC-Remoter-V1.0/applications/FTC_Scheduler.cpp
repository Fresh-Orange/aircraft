/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Scheduler.cpp
 * ����    ��������
**********************************************************************************/
#include "FTC_Scheduler.h"

FTC_Scheduler scheduler;


static void FTC_Loop_1000Hz(void)	//1msִ��һ��
{
	rc.DataGet();
}

static void FTC_Loop_500Hz(void)	//2msִ��һ��
{	
	//���´���������	
	imu.updateSensor();		
	
	//�����������̬
	imu.getAttitude();	
}

static void FTC_Loop_200Hz(void)	//5msִ��һ��
{
	rc.DataCalculate();
}

static void FTC_Loop_100Hz(void)	//10msִ��һ��
{
	rc.KeyCheck();
	
	//LEDָʾ�ƿ���
	ftc.Pilot_Light();
	
}

static void FTC_Loop_30Hz(void)	//30msִ��һ��
{
	dt.Data_Exchange();
}

void FTC_Loop(void)
{
	
	//����Ƿ��н��յ���������
	dt.Check_Event();
	
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
	if(scheduler.cnt_30ms >= 30){		
		FTC_Loop_30Hz();
		scheduler.cnt_30ms = 0;
	}
}

FTC_Scheduler::FTC_Scheduler()
{
	cnt_1ms = cnt_2ms = cnt_5ms = cnt_10ms = cnt_30ms	= 0;
}



/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
