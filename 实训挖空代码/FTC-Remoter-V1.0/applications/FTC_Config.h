#ifndef __FTC_CONFIG_H
#define __FTC_CONFIG_H

#include "board.h"
#include "FTC_Scheduler.h"
#include "FTC_DT.h"
#include "FTC_RC.h"
#include "FTC_Param.h"
#include "FTC_Filter.h"
#include "FTC_IMU.h"

/*----------------------IMU--------------------*/
#define IMU_LOOP_TIME					2000	//单位为uS

#define ACC_1G 			4096		//由加速度计的量程确定
#define ACC_LPF_CUT 30.0f		//加速度低通滤波器截止频率10Hz

#define GYRO_CF_TAU 1.2f
/*---------------------------------------------*/


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
			uint8_t ARMED;
			uint8_t STICKMODE;
			uint8_t ACCELMODE;
	}f;
	
	//指示灯
	void Pilot_Light(void);
	
};

extern FTC_Config ftc;

#endif

