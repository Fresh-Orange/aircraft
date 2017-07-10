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
	uint16_t kP;
	uint16_t kI;
	uint16_t kD;
	
	//����PID�����ֵ
	int32_t get_pid(int32_t error, uint16_t dt);
	int32_t get_pi(int32_t error, uint16_t dt);
	int32_t get_p(int32_t error);
	int32_t get_i(int32_t error, uint16_t dt);
	int32_t get_d(int32_t error, uint16_t dt);
	
	//���ֿ�������ֵ����
	void reset_I(void);
	
	void set_pid(const uint16_t p,
									 const uint16_t i,
									 const uint16_t d,
									 const uint32_t  imaxval) {
			kP = p; kI = i; kD = d; imax = imaxval;
	}
	
private:
	
	//��������
	uint32_t imax;
	//��������ֵ
	int32_t integrator; 
	//��һ�ε��������
	int32_t last_error;
};



#endif

