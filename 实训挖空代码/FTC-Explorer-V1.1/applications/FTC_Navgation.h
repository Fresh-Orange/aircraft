#ifndef __FTC_NAVGATION_H
#define __FTC_NAVGATION_H

#include "FTC_Config.h"

class FTC_Navgation : public FTC_Filter
{
	
public:

	FTC_Navgation();

	int16_t baroVel, ultraVel;	

	int32_t baroAlt, ultraAlt, ultraAltRaw; 

	Vector3f accel, velocity, position;

	Vector3f accel_last, velocity_last;

	int32_t velRatio;

	LPF2ndData_t baro_lpf_2nd, ultra_lpf_2nd;


	void Init(void);
	void update(void);
	void Reset(void);
	
private:
	
	struct Factor{
			float velxy_cf;
			float velz_baro_cf;
			float velz_ultra_cf;
			float velz_cf_ki;
			float baro_cf;	
			float ultra_cf;		
	}factor;
	
	float accVelScale;
	//���ٶ�����
	Vector3f accDeadband;
	//��ʵ�������ٶ�
	int32_t GRAVITY_MSS;

 	int32_t lastBaroAlt, lastUltraAlt;

	int32_t baroOffset;
	
	uint8_t ULTRA_IS_OK;
	
	//�˲���������ʼ��
	void filter_Init(void);
	//��������ʼ��
	void sensor_Init(void);

};

extern FTC_Navgation nav;

#endif
