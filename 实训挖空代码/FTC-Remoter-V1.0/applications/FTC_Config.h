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
#define IMU_LOOP_TIME					2000	//��λΪuS

#define ACC_1G 			4096		//�ɼ��ٶȼƵ�����ȷ��
#define ACC_LPF_CUT 30.0f		//���ٶȵ�ͨ�˲�����ֹƵ��10Hz

#define GYRO_CF_TAU 1.2f
/*---------------------------------------------*/


/*-------------------�������ݷ��ͷ�ʽѡ��-----------------*/
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
	
	//ָʾ��
	void Pilot_Light(void);
	
};

extern FTC_Config ftc;

#endif

