#ifndef __FTC_PID_H
#define __FTC_PID_H

#include <stdint.h>
#include "FTC_Math.h"

class FTC_PID
{
	
public:
	
	FTC_PID(){
		kP = kI = kD = 0;
		imax = 0;
	}

	//PID����
	float kP;
	float kI;
	float kD;
	
	//����PID�����ֵ
	int32_t get_pid(int32_t error, float dt);
	int32_t get_pi(int32_t error, float dt);
	int32_t get_p(int32_t error);
	int32_t get_i(int32_t error, float dt);
	int32_t get_d(int32_t error, float dt);
	
	//���ֿ�������ֵ����
	void reset_I(void);
	
	void set_pid(const float p,
									 const float i,
									 const float d,
									 const float  imaxval) {
			kP = p; kI = i; kD = d; imax = imaxval;
	}
	
private:
	
	//��������
	float imax;
	//��������ֵ
	float integrator; 
	//��һ�ε��������
	int32_t last_error;
	//��һ�ε�΢��ֵ����ͨ�˲�������
	float	last_derivative;   

	/// ΢����ĵ�ͨ�˲�����ֹƵ��
	static const float  _filter;

};



#endif

