#ifndef __FTC_FILTER_H
#define __FTC_FILTER_H

#include "FTC_Math.h"



class FTC_Filter
{
	
public:

	FTC_Filter(){
	}
	
	struct LPF2ndData_t
	{
		float b0;
		float a1;
		float a2;
		Vector3f preout;
		Vector3f lastout;
	};
	
	//一阶低通滤波器系数计算
	float LPF_1st_Factor_Cal(float deltaT, float Fcut);
	
	//二阶低通滤波器系数计算
	void LPF_2nd_Factor_Cal(LPF2ndData_t* lpf_data);
	
	//互补滤波器系数计算
	float CF_Factor_Cal(float deltaT, float tau);
	
	//一阶低通滤波器
	Vector3f LPF_1st(Vector3f oldData, Vector3f newData, float lpf_factor);
	
	//二阶低通滤波器
	Vector3f LPF_2nd(LPF2ndData_t* lpf_2nd, Vector3f newData);
	
	//一阶互补滤波器
	Vector3f CF_1st(Vector3f gyroData, Vector3f accData, float cf_factor);
	


};


#endif

