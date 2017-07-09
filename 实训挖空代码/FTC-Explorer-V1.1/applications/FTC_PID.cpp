/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_PID.cpp
 * 描述    ：PID运算库
**********************************************************************************/
#include "FTC_PID.h"

// Examples for _filter:
// f_cut = 10 Hz -> _filter = 15.9155e-3
// f_cut = 15 Hz -> _filter = 10.6103e-3
// f_cut = 20 Hz -> _filter =  7.9577e-3
// f_cut = 25 Hz -> _filter =  6.3662e-3
// f_cut = 30 Hz -> _filter =  5.3052e-3
const float  FTC_PID::_filter = 7.9577e-3; // 由 "1 / ( 2 * PI * f_cut )"这个公式计算得来;


int32_t FTC_PID::get_p(int32_t error)
{
    return (float)error * kP;
}

int32_t FTC_PID::get_i(int32_t error, float dt)
{
    if((kI != 0) && (dt != 0)) {
        integrator += ((float)error * kI) * dt;
				//积分限幅
				integrator = constrain_float(integrator, -imax, +imax);		
				
        return integrator;
    }
    return 0;
}

void FTC_PID::reset_I(void)
{
	integrator = 0;
}

int32_t FTC_PID::get_d(int32_t error, float dt)
{
	if ((kD != 0) && (dt != 0)) {
			float derivative;
	if (isnan(last_derivative)) {		
		derivative = 0;
		last_derivative = 0;
	} else {
		derivative = (error - last_error) / dt;
	}

	// 一阶离散低通滤波器，降低高频噪声对控制器的干扰
	derivative = last_derivative +
								(dt / ( _filter + dt)) * (derivative - last_derivative);

	last_error	= error;
	last_derivative = derivative;

	return kD * derivative;
	}
    return 0;
}

int32_t FTC_PID::get_pi(int32_t error, float dt)
{
    return get_p(error) + get_i(error, dt);
}

int32_t FTC_PID::get_pid(int32_t error, float dt)
{
    return get_p(error) + get_i(error, dt) + get_d(error, dt);
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
