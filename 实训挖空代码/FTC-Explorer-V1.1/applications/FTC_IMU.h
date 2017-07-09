#ifndef __FTC_IMU_H
#define __FTC_IMU_H

#include "FTC_Config.h"

class FTC_IMU : public FTC_Filter
{
	
public:

	FTC_IMU();
	
	//欧拉角表示的飞行器姿态
	Vector3f angle;
	
	Vector3f Gyro, Gyro_lpf, Acc, Acc_lpf; 

	LPF2ndData_t Acc_lpf_2nd, Gyro_lpf_2nd;

	LPF4thData_t Acc_lpf_4th, Gyro_lpf_4th;

	float magHold;
	
	void Init();
	
	//更新传感器数据
	void updateSensor();	
	
	//计算飞行器姿态
	void getAttitude();
	
	Vector3f Get_Accel_Ef();
		
	
private:

	Quaternion Q;

	int32_t accRatio;

	float getDeltaT(uint32_t time);

	//基于余弦矩阵和互补滤波的姿态解算
	void DCM_CF(Vector3f gyro,Vector3f acc, float deltaT);
	//基于四元数和互补滤波的姿态解算
	void Quaternion_CF(Vector3f gyro,Vector3f acc, float deltaT);

	//滤波器参数初始化
	void filter_Init();
	//传感器初始化
	void sensor_Init();

};

extern FTC_IMU imu;

#endif
