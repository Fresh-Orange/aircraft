#ifndef __FTC_DRV_MS5611_H
#define	__FTC_DRV_MS5611_H

#include "board.h"

#define PROM_NB	8

class FTC_MS5611:public FTC_I2C_Soft
{
public:
	
	FTC_MS5611(){
	}
	
	//气压计初始化
	void Init(void);
	//读取气压计数据
	void Update(void);	
	//返回气压高度
	int32_t Get_BaroAlt(void);

private:
	
	int32_t baroAlt;
	int32_t baro_Offset;
	uint32_t ms5611_ut;  // static result of temperature measurement
	uint32_t ms5611_up;  // static result of pressure measurement
	uint16_t ms5611_prom[PROM_NB];  // on-chip ROM
	uint8_t t_rxbuf[3],p_rxbuf[3];

	void Reset(void);
	bool Read_Prom(void);
	void Start_T(void);
	void Start_P(void);
	void Read_Adc_T(void);
	void Read_Adc_P(void);
	void BaroAltCalculate(void);

};

extern FTC_MS5611 ms5611;

#endif
