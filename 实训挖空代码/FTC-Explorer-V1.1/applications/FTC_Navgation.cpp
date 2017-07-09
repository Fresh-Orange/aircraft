/******************** (C) COPYRIGHT 2015 FTC *******************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Navgation.cpp
 * 描述    ：飞行器组合导航
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
	
	//发送US100超声波测距信号
	if(ultraCnt == 10 || uart3.RxState3 == 2){	
			uart3.RxState3 = 0;
			uart3.Put_Char(0x55);		
			ultraCnt = 0;		

			deltaT = (GetSysTime_us() - lastTimeUltra) * 1e-6;
			lastTimeUltra = GetSysTime_us();
		
			//获取超声波高度并低通滤波
			ultraAlt = Ultra_LPF_2nd(&ultra_lpf_2nd, (float)ultraAltRaw);	
			//计算超声波变化速度
			ultraVel = (ultraAlt - lastUltraAlt) / deltaT;
			lastUltraAlt = ultraAlt;
			//限制气压变化的速度，正负800cm/s
			ultraVel = constrain_int32(ultraVel, -8000, 8000);    
			//增加死区减小噪声
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
		//气压传感器数据更新
		ms5611.Update();
	}
	else if(baroCnt == 10){
		deltaT = (GetSysTime_us() - lastTimeBaro) * 1e-6;
		lastTimeBaro = GetSysTime_us();
		baroCnt = 0;
		
		//气压传感器数据更新
		ms5611.Update();
		
		//获取气压高度并低通滤波（已校零偏）
		baroAlt = Baro_LPF_2nd(&baro_lpf_2nd, (float)ms5611.Get_BaroAlt());	
		
		//计算气压变化速度
		baroVel = (baroAlt -lastBaroAlt) / deltaT;
		lastBaroAlt = baroAlt;
		//限制气压变化的速度，正负800cm/s
    baroVel = constrain_int32(baroVel, -800, 800);    
		//增加死区减小噪声
    baroVel = applyDeadband(baroVel, 10);    

		if(ULTRA_IS_OK){
			velocity_I.z += ultraVel - velocity.z;
			//z轴速度
			velocity.z = velocity.z * factor.velz_ultra_cf + ultraVel * 0.1 * (1 - factor.velz_ultra_cf) ;//+ velocity_I.z * factor.velz_cf_ki;
			//z轴高度	
			position.z = position.z * factor.ultra_cf + ultraAlt * 0.1 * (1 - factor.ultra_cf);			
			
			baroOffset += 0.001 * (float)(ultraAlt * 0.1  - baroAlt);
		}
		else{
			//互补滤波校正积分偏移
			velocity_I.z += baroVel - velocity.z;
			//z轴速度
			velocity.z = velocity.z * factor.velz_baro_cf + baroVel * (1 - factor.velz_baro_cf) + velocity_I.z * factor.velz_cf_ki;
			//z轴高度	
			position.z = position.z * factor.baro_cf + (baroAlt + baroOffset) * (1 - factor.baro_cf);
			
			if(abs(baroAlt - position.z)> 200)
				position.z = baroAlt;
		}
	}
	baroCnt++;

	deltaT = (GetSysTime_us() - lastTimeInertia) * 1e-6;
	lastTimeInertia = GetSysTime_us();
	
	//获取飞行器的加速度在地理坐标系的投影
	accel_ef = imu.Get_Accel_Ef();
	
	//计算真实重力加速度
	if (!ftc.f.ARMED) {
			GRAVITY_MSS -= GRAVITY_MSS / 64;
			GRAVITY_MSS += accel_ef.z;
	}
	accel_ef.z -= GRAVITY_MSS / 64;
	
	//增加死区，减小加速度积分漂移
	accel.x = (float)applyDeadband(accel_ef.x, accDeadband.x) * accVelScale;
	accel.y = -(float)applyDeadband(accel_ef.y, accDeadband.y) * accVelScale; //y轴加速度取反，是因为要将西北天坐标系转化为东北天
	accel.z = (float)applyDeadband(accel_ef.z, accDeadband.z) * accVelScale;
	
	//二阶龙格库塔积分出位移 s += v * t
	position += (velocity + velocity_last) * deltaT * 0.5;
	
	//更新速度历史值
	velocity_last = velocity;
	
	//二阶龙格库塔积分出速度 v = a*t
	velocity += (accel + accel_last) * deltaT * 0.5;	
	//更新加速度历史值
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
	//初始化气压计
	ms5611.Init();
}
