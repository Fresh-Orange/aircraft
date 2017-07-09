/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Filter.cpp
 * 描述    ：滤波器相关函数
**********************************************************************************/
#include "FTC_Filter.h"


/*----------------------一阶低通滤波器系数计算-------------------------*/
float FTC_Filter::LPF_1st_Factor_Cal(float deltaT, float Fcut)
{
	return deltaT / (deltaT + 1 / (2 * M_PI * Fcut));
}

/*----------------------一阶低通滤波器------------------------*/
Vector3f FTC_Filter::LPF_1st(Vector3f oldData, Vector3f newData, float lpf_factor)
{
	return oldData * (1 - lpf_factor) + newData * lpf_factor;
}

/*----------------------二阶低通滤波器系数计算-------------------------*/
void FTC_Filter::LPF_2nd_Factor_Cal(LPF2ndData_t* lpf_data)
{
	//截止频率:30Hz 采样频率:500Hz
	lpf_data->b0 = 0.1883633f;
	lpf_data->a1 = 1.023694f;
	lpf_data->a2 = 0.2120577f;
}

/*----------------------二阶低通滤波器------------------------*/
Vector3f FTC_Filter::LPF_2nd(LPF2ndData_t* lpf_2nd, Vector3f newData)
{
	Vector3f lpf_2nd_data;
	
	lpf_2nd_data = newData * lpf_2nd->b0 + lpf_2nd->lastout * lpf_2nd->a1 - lpf_2nd->preout * lpf_2nd->a2;
	lpf_2nd->preout = lpf_2nd->lastout;
	lpf_2nd->lastout = lpf_2nd_data;
	
	return lpf_2nd_data;
}

/*----------------------互补滤波器系数计算-------------------------*/
float FTC_Filter::CF_Factor_Cal(float deltaT, float tau)
{
	return tau / (deltaT + tau);
}

/*----------------------一阶互补滤波器-----------------------------*/
Vector3f FTC_Filter::CF_1st(Vector3f gyroData, Vector3f accData, float cf_factor)
{ 
	return (gyroData * cf_factor + accData *(1 - cf_factor));	
}





/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
