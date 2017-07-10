#ifndef __FTC_SCHEDULER_H
#define __FTC_SCHEDULER_H

#include "board.h"
#include "FTC_Config.h"

class FTC_Scheduler
{
public:
	//���캯��
	FTC_Scheduler();
	//����ʱ����������
	uint16_t cnt_1ms,cnt_2ms,cnt_5ms,cnt_10ms,cnt_20ms;

};

void FTC_Loop(void);

extern FTC_Scheduler scheduler;

#endif
