#ifndef __FTC_RC_H
#define __FTC_RC_H

#include "FTC_Config.h"

enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4
};

class FTC_RC
{
	
public:
	
	FTC_RC();

	uint16_t Data[8];
	int16_t DataTrim[8];
	float DataRaw[8];

	uint16_t KeyDataL,KeyDataR;
	float KeyDataRawL,KeyDataRawR;

	//������ʼ��
	void Init(void);
	
	//��ADC����ֵתΪң��ͨ������
	void DataGet(void);

	void DataCalculate(void);

	void KeyCheck(void);

private:

};

extern FTC_RC rc;

#endif



