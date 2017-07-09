#ifndef __FTC_CONFIG_H
#define __FTC_CONFIG_H

#include "board.h"
#include "FTC_PID.h"
#include "FTC_Filter.h"
#include "FTC_IMU.h"
#include "FTC_Scheduler.h"
#include "FTC_DT.h"
#include "FTC_Motor.h"
#include "FTC_RC.h"
#include "FTC_FlyControl.h"
#include "FTC_Navgation.h"
#include "FTC_Param.h"

/*----------------------IMU--------------------*/
#define FTC_IMU_USE_DCM_CF
//#define FTC_IMU_USE_Quaternions_CF

//#define FTC_IMU_USE_LPF_1st
#define FTC_IMU_USE_LPF_2nd


#define IMU_LOOP_TIME					2000	//单位为uS
#define PID_INNER_LOOP_TIME		2000	//单位为us
#define PID_OUTER_LOOP_TIME		5000	//单位为us

#define ACC_1G 			4096		//由加速度计的量程确定
#define ACC_LPF_CUT 50.0f		//加速度低通滤波器截止频率50Hz
#define GYRO_LPF_CUT 130.0f		//陀螺仪低通滤波器截止频率130Hz

#define GYRO_CF_TAU 1.8f
/*---------------------------------------------*/


/*----------------------Navgation--------------------*/
#define BARO_LOOP_TIME					20000	//单位为uS
#define BARO_LPF_CUT	5.0f 

#define ULTRA_LOOP_TIME					20000	//单位为uS
#define ULTRA_LPF_CUT	50.0f 
/*--------------------------------------------------------*/


/*-------------------无线数据发送方式选择-----------------*/
//#define FTC_DT_USE_Bluetooth
#define FTC_DT_USE_NRF24l01
/*--------------------------------------------------------*/


class FTC_Config
{
	
public:
	
	FTC_Config();

	class Factor{
		public:		
			float acc_lpf;		
			float gyro_cf;		
	}factor;

	class Flag{
		public:
			uint8_t CALIBRATED;
			uint8_t ARMED;
			uint8_t FAILSAFE;
			uint8_t ANGLE_MODE;
			uint8_t ALTHOLD;
			uint8_t LOWPOWER;
	}f;
	
	//指示灯
	void Pilot_Light(void);
	
};

extern FTC_Config ftc;

#endif

