#ifndef __FTC_PARAM_H
#define __FTC_PARAM_H

#include "FTC_Config.h"


class FTC_Param
{
	
public:
	
	void Init(void);

	void SAVE_RC_OFFSET(void);

private:
	
	void READ_RC_OFFSET(void);
	void READ_CONF(void);
};

extern FTC_Param param;

#endif

