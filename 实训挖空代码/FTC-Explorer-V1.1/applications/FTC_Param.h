#ifndef __FTC_PARAM_H
#define __FTC_PARAM_H

#include "FTC_Config.h"


class FTC_Param
{
	
public:
	
	void Init(void);

	void SAVE_ACC_OFFSET(void);
	void SAVE_GYRO_OFFSET(void);
	void SAVE_PID(void);

private:
	
	void SAVE_FirstInitFlag(void);
	uint16_t READ_FirstInitFlag(void);
	void READ_ACC_OFFSET(void);
	void READ_GYRO_OFFSET(void);
	void READ_PID(void);
	void READ_CONF(void);
};

extern FTC_Param param;

#endif

