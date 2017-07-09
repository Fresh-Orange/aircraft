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

	//PID参数
	float kP;
	float kI;
	float kD;
	
	//返回PID计算的值
	int32_t get_pid(int32_t error, float dt);
	int32_t get_pi(int32_t error, float dt);
	int32_t get_p(int32_t error);
	int32_t get_i(int32_t error, float dt);
	int32_t get_d(int32_t error, float dt);
	
	//积分控制器的值清零
	void reset_I(void);
	
	void set_pid(const float p,
									 const float i,
									 const float d,
									 const float  imaxval) {
			kP = p; kI = i; kD = d; imax = imaxval;
	}
	
private:
	
	//积分上限
	float imax;
	//积分器的值
	float integrator; 
	//上一次的误差输入
	int32_t last_error;
	//上一次的微分值，低通滤波计算用
	float	last_derivative;   

	/// 微分项的低通滤波器截止频率
	static const float  _filter;

};



#endif

