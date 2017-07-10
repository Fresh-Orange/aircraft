#ifndef __FTC_FILTER_H
#define __FTC_FILTER_H

#include "FTC_Math.h"



class FTC_Filter
{
	
public:

	FTC_Filter(){
	}
	
	struct LPF2ndData_t{
		float b0;
		float a1;
		float a2;
		Vector3f preout;
		Vector3f lastout;
	};
	
	struct LPF4thData_t{
		double b0, b1, b2, b3, b4;
		double a1, a2, a3, a4;
		Vector3d in1, in2, in3, in4;
		Vector3d out1, out2, out3, out4;
	};	
	
	//һ�׵�ͨ�˲���ϵ������
	float LPF_1st_Factor_Cal(float deltaT, float Fcut);
	
	//���׵�ͨ�˲���ϵ������
	void LPF_2nd_Factor_Cal(float deltaT, float Fcut, LPF2ndData_t* lpf_data);
	
	//�����˲���ϵ������
	float CF_Factor_Cal(float deltaT, float tau);
	
	//һ�׵�ͨ�˲���
	Vector3f LPF_1st(Vector3f oldData, Vector3f newData, float lpf_factor);
	
	//���׵�ͨ�˲���
	Vector3f LPF_2nd(LPF2ndData_t* lpf_2nd, Vector3f newData);
	
	int32_t Baro_LPF_2nd(LPF2ndData_t* lpf_2nd, float newData);
	int32_t Ultra_LPF_2nd(LPF2ndData_t* lpf_2nd, float newData);
	
	//һ�׻����˲���
	Vector3f CF_1st(Vector3f gyroData, Vector3f accData, float cf_factor);
	


};


#endif

