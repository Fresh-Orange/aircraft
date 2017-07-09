/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Scheduler.cpp
 * 描述    ：任务处理
**********************************************************************************/
#include "FTC_Scheduler.h"

FTC_Scheduler scheduler;

static void FTC_Loop_1000Hz(void)	//1ms执行一次
{
	//检查是否有接收到无线数据
	dt.Check_Event();
	
	//发送飞行器数据
	dt.Data_Exchange();
}

static void FTC_Loop_500Hz(void)	//2ms执行一次
{	
	//更新传感器数据	
	imu.updateSensor();		
	
	//计算飞行器姿态
	imu.getAttitude();
	
	//飞行器姿态内环控制
	fc.Attitude_Inner_Loop();	
	
	//飞行器高度传感器数据更新
	nav.update();
}

static void FTC_Loop_200Hz(void)	//5ms执行一次
{
	//飞行器姿态外环控制
	fc.Attitude_Outter_Loop();
}

static void FTC_Loop_100Hz(void)	//10ms执行一次
{
	//电池电压测量
	FTC_ADC_Battery();
}

static void FTC_Loop_50Hz(void)	//20ms执行一次
{
	//遥控通道数据处理
	rc.Cal_Command();
	
	//摇杆位置检查
	rc.check_sticks();
	
	//辅助通道检测
	rc.CheckAUX();
	
	//失控保护检查
	dt.Failsafe_Check();
	
	//LED指示灯控制
	ftc.Pilot_Light();
	
	//飞行器高度外环控制
	fc.Altitude_Outter_Loop();	
	
	//飞行器高度内环控制
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
