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
void FTC_Filter::LPF_2nd_Factor_Cal(float deltaT, float Fcut, LPF2ndData_t* lpf_data)
{
	float a = 1 / (2 * M_PI * Fcut * deltaT);
	lpf_data->b0 = 1 / (a*a + 3*a + 1);
	lpf_data->a1 = (2*a*a + 3*a) / (a*a + 3*a + 1);
	lpf_data->a2 = (a*a) / (a*a + 3*a + 1);
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

int32_t FTC_Filter::Baro_LPF_2nd(LPF2ndData_t* lpf_2nd, float newData)
{
	float baroAlt;
	static float baro_lastout, baro_preout;
	
	baroAlt = newData * lpf_2nd->b0 + baro_lastout * lpf_2nd->a1 - baro_preout * lpf_2nd->a2;
	baro_preout = baro_lastout;
	baro_lastout = baroAlt;
	
	return (int32_t)baroAlt;
}

int32_t FTC_Filter::Ultra_LPF_2nd(LPF2ndData_t* lpf_2nd, float newData)
{
	float ultraAlt;
	static float ultra_lastout, ultra_preout;
	
	ultraAlt = newData * lpf_2nd->b0 + ultra_lastout * lpf_2nd->a1 - ultra_preout * lpf_2nd->a2;
	ultra_preout = ultra_lastout;
	ultra_lastout = ultraAlt;
	
	return (int32_t)ultraAlt;
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
