/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Scheduler.cpp
 * 描述    ：任务处理
**********************************************************************************/
#include "FTC_Scheduler.h"

FTC_Scheduler scheduler;


static void FTC_Loop_1000Hz(void)	//1ms执行一次
{
	rc.DataGet();
}

static void FTC_Loop_500Hz(void)	//2ms执行一次
{	
	//更新传感器数据	
	imu.updateSensor();		
	
	//计算飞行器姿态
	imu.getAttitude();	
}

static void FTC_Loop_200Hz(void)	//5ms执行一次
{
	rc.DataCalculate();
}

static void FTC_Loop_100Hz(void)	//10ms执行一次
{
	rc.KeyCheck();
	
	//LED指示灯控制
	ftc.Pilot_Light();
	
}

static void FTC_Loop_30Hz(void)	//30ms执行一次
{
	dt.Data_Exchange();
}

void FTC_Loop(void)
{
	
	//检查是否有接收到无线数据
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
