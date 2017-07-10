/******************** (C) COPYRIGHT 2015 FTC *******************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Navgation.cpp
 * ����    ����������ϵ���
**********************************************************************************/
#include "FTC_Navgation.h"
#include <math.h>

FTC_Navgation nav;

FTC_Navgation::FTC_Navgation()
{
	accDeadband(8,8,8);

	accVelScale = 9.7925f * 100 / ACC_1G;
	GRAVITY_MSS = ACC_1G * 64;
	
	factor.velxy_cf = 0.8f;
	factor.velz_baro_cf = 0.995f;
	factor.velz_ultra_cf = 0.99;
	factor.velz_cf_ki = 0.005f;
	factor.baro_cf = 0.985f;
	factor.ultra_cf = 0.95f;	
}


void FTC_Navgation::Init(void)
{
	filter_Init();
	sensor_Init();
}

void FTC_Navgation::update(void)
{
	static int baroCnt = 0, ultraCnt = 0;
	static uint32_t lastTimeInertia, lastTimeBaro, lastTimeUltra, lastUltraAlt;
	static uint8_t ultraSameCnt = 0;
	float	deltaT;
	
	Vector3f accel_ef, velocity_I;
	
	//����US100����������ź�
	if(ultraCnt == 10 || uart3.RxState3 == 2){	
			uart3.RxState3 = 0;
			uart3.Put_Char(0x55);		
			ultraCnt = 0;		

			deltaT = (GetSysTime_us() - lastTimeUltra) * 1e-6;
			lastTimeUltra = GetSysTime_us();
		
			//��ȡ�������߶Ȳ���ͨ�˲�
			ultraAlt = Ultra_LPF_2nd(&ultra_lpf_2nd, (float)ultraAltRaw);	
			//���㳬�����仯�ٶ�
			ultraVel = (ultraAlt - lastUltraAlt) / deltaT;
			lastUltraAlt = ultraAlt;
			//������ѹ�仯���ٶȣ�����800cm/s
			ultraVel = constrain_int32(ultraVel, -8000, 8000);    
			//����������С����
			ultraVel = applyDeadband(ultraVel, 100); 		
		
			if(ultraAltRaw == lastUltraAlt)
				ultraSameCnt++;
			else
				ultraSameCnt = 0;
			
			lastUltraAlt = ultraAltRaw;
			
			if(ultraAlt > 0 && ultraAlt < 1000 && ultraSameCnt < 30){
				ULTRA_IS_OK = 1;
			}
			else{
				ULTRA_IS_OK = 0;
			}
			
	}	
	ultraCnt++;
	
	if(baroCnt == 5){
		//��ѹ���������ݸ���
		ms5611.Update();
	}
	else if(baroCnt == 10){
		deltaT = (GetSysTime_us() - lastTimeBaro) * 1e-6;
		lastTimeBaro = GetSysTime_us();
		baroCnt = 0;
		
		//��ѹ���������ݸ���
		ms5611.Update();
		
		//��ȡ��ѹ�߶Ȳ���ͨ�˲�����У��ƫ��
		baroAlt = Baro_LPF_2nd(&baro_lpf_2nd, (float)ms5611.Get_BaroAlt());	
		
		//������ѹ�仯�ٶ�
		baroVel = (baroAlt -lastBaroAlt) / deltaT;
		lastBaroAlt = baroAlt;
		//������ѹ�仯���ٶȣ�����800cm/s
    baroVel = constrain_int32(baroVel, -800, 800);    
		//����������С����
    baroVel = applyDeadband(baroVel, 10);    

		if(ULTRA_IS_OK){
			velocity_I.z += ultraVel - velocity.z;
			//z���ٶ�
			velocity.z = velocity.z * factor.velz_ultra_cf + ultraVel * 0.1 * (1 - factor.velz_ultra_cf) ;//+ velocity_I.z * factor.velz_cf_ki;
			//z��߶�	
			position.z = position.z * factor.ultra_cf + ultraAlt * 0.1 * (1 - factor.ultra_cf);			
			
			baroOffset += 0.001 * (float)(ultraAlt * 0.1  - baroAlt);
		}
		else{
			//�����˲�У������ƫ��
			velocity_I.z += baroVel - velocity.z;
			//z���ٶ�
			velocity.z = velocity.z * factor.velz_baro_cf + baroVel * (1 - factor.velz_baro_cf) + velocity_I.z * factor.velz_cf_ki;
			//z��߶�	
			position.z = position.z * factor.baro_cf + (baroAlt + baroOffset) * (1 - factor.baro_cf);
			
			if(abs(baroAlt - position.z)> 200)
				position.z = baroAlt;
		}
	}
	baroCnt++;

	deltaT = (GetSysTime_us() - lastTimeInertia) * 1e-6;
	lastTimeInertia = GetSysTime_us();
	
	//��ȡ�������ļ��ٶ��ڵ�������ϵ��ͶӰ
	accel_ef = imu.Get_Accel_Ef();
	
	//������ʵ�������ٶ�
	if (!ftc.f.ARMED) {
			GRAVITY_MSS -= GRAVITY_MSS / 64;
			GRAVITY_MSS += accel_ef.z;
	}
	accel_ef.z -= GRAVITY_MSS / 64;
	
	//������������С���ٶȻ���Ư��
	accel.x = (float)applyDeadband(accel_ef.x, accDeadband.x) * accVelScale;
	accel.y = -(float)applyDeadband(accel_ef.y, accDeadband.y) * accVelScale; //y����ٶ�ȡ��������ΪҪ������������ϵת��Ϊ������
	accel.z = (float)applyDeadband(accel_ef.z, accDeadband.z) * accVelScale;
	
	//��������������ֳ�λ�� s += v * t
	position += (velocity + velocity_last) * deltaT * 0.5;
	
	//�����ٶ���ʷֵ
	velocity_last = velocity;
	
	//��������������ֳ��ٶ� v = a*t
	velocity += (accel + accel_last) * deltaT * 0.5;	
	//���¼��ٶ���ʷֵ
	accel_last = accel;
}

void FTC_Navgation::Reset(void)
{
	accel(0,0,0);
	velocity(0,0,0);
	position(0,0,0);
}

void FTC_Navgation::filter_Init()
{
	LPF_2nd_Factor_Cal(BARO_LOOP_TIME * 1e-6, BARO_LPF_CUT, &baro_lpf_2nd);
	
	LPF_2nd_Factor_Cal(ULTRA_LOOP_TIME * 1e-6, ULTRA_LPF_CUT, &ultra_lpf_2nd);
}

void FTC_Navgation::sensor_Init()
{
	//��ʼ����ѹ��
	ms5611.Init();
}
