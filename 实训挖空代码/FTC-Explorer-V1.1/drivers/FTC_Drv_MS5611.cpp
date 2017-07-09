/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_MS5611.cpp
 * 描述    ：MS5611
**********************************************************************************/
#include "FTC_Drv_MS5611.h"

#define MS5611_ADDR             0xee //    0x77

#define CMD_RESET               0x1E // ADC reset command
#define CMD_ADC_READ            0x00 // ADC read command
#define CMD_ADC_CONV            0x40 // ADC conversion command
#define CMD_ADC_D1              0x00 // ADC D1 conversion
#define CMD_ADC_D2              0x10 // ADC D2 conversion
#define CMD_ADC_256             0x00 // ADC OSR=256
#define CMD_ADC_512             0x02 // ADC OSR=512
#define CMD_ADC_1024            0x04 // ADC OSR=1024
#define CMD_ADC_2048            0x06 // ADC OSR=2048
#define CMD_ADC_4096            0x08 // ADC OSR=4096
#define CMD_PROM_RD             0xA0 // Prom read command
#define PROM_NB                 8
#define MS5611_OSR							0x08	//CMD_ADC_4096

#define BARO_CAL_CNT 200

FTC_MS5611 ms5611;

void FTC_MS5611::Reset(void)
{
    I2C_Single_Write(MS5611_ADDR, CMD_RESET, 1);
}

bool FTC_MS5611::Read_Prom(void)
{
	uint8_t rxbuf[2] = { 0, 0 };
	u8 check = 0;

	for (u8 i = 0; i < PROM_NB; i++)
	{
		check += I2C_Mult_Read(MS5611_ADDR, CMD_PROM_RD + i * 2, rxbuf, 2); // send PROM READ command
		ms5611_prom[i] = rxbuf[0] << 8 | rxbuf[1];
	}

	if(check==PROM_NB)
		return true;
	else
		return false;
}

void FTC_MS5611::Init(void)
{
	DelayMs(10);
	//传感器复位
	Reset();
	DelayMs(3);
	Read_Prom();
	//开始读取温度
	Start_T();
}

void FTC_MS5611::Read_Adc_T(void)
{
	I2C_Mult_Read(MS5611_ADDR, CMD_ADC_READ, t_rxbuf, 3); // read ADC
}

void FTC_MS5611::Read_Adc_P(void)
{
	I2C_Mult_Read(MS5611_ADDR, CMD_ADC_READ, p_rxbuf, 3); // read ADC
}

void FTC_MS5611::Start_T(void)
{
	I2C_Single_Write(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D2 + MS5611_OSR, 1); // D2 (temperature) conversion start!
}

void FTC_MS5611::Start_P(void)
{
    I2C_Single_Write(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D1 + MS5611_OSR, 1); // D1 (pressure) conversion start!
}


void FTC_MS5611::Update(void)
{
	static int state = 0;
	
	if (state) {
			Read_Adc_P();
			Start_T();
			BaroAltCalculate();
			state = 0;
	} else {
			Read_Adc_T();
			Start_P();
			state = 1;
	}
}

void FTC_MS5611::BaroAltCalculate(void)
{
    int32_t temperature, off2 = 0, sens2 = 0, delt;
    int32_t pressure;
	
		static vs32 sum_temp = 0;
		static u8 sum_cnt = BARO_CAL_CNT + 10;
	
		ms5611_ut = (t_rxbuf[0] << 16) | (t_rxbuf[1] << 8) | t_rxbuf[2];
		ms5611_up = (p_rxbuf[0] << 16) | (p_rxbuf[1] << 8) | p_rxbuf[2];
		
    int32_t dT = ms5611_ut - ((uint32_t)ms5611_prom[5] << 8);
    int64_t off = ((uint32_t)ms5611_prom[2] << 16) + (((int64_t)dT * ms5611_prom[4]) >> 7);
    int64_t sens = ((uint32_t)ms5611_prom[1] << 15) + (((int64_t)dT * ms5611_prom[3]) >> 8);
    temperature = 2000 + (((int64_t)dT * ms5611_prom[6]) >> 23);

    if (temperature < 2000) { // temperature lower than 20degC 
        delt = temperature - 2000;
        delt = delt * delt;
        off2 = (5 * delt) >> 1;
        sens2 = (5 * delt) >> 2;
        if (temperature < -1500) { // temperature lower than -15degC
            delt = temperature + 1500;
            delt = delt * delt;
            off2  += 7 * delt;
            sens2 += (11 * delt) >> 1;
        }
    }
    off  -= off2; 
    sens -= sens2;
    pressure = (((ms5611_up * sens ) >> 21) - off) >> 15;
		pressure = (int)((1.0f - pow(pressure / 101325.0f, 0.190295f)) * 4433000.0f); // centimeter
		if(!(baro_Offset == 0))
			baroAlt = pressure - baro_Offset;
		
		if(sum_cnt)
		{
			sum_cnt--;
			if(sum_cnt < BARO_CAL_CNT)
				sum_temp += pressure;
			if(sum_cnt==0)
				baro_Offset = sum_temp / (BARO_CAL_CNT - 1);
		}
}

int32_t FTC_MS5611::Get_BaroAlt(void)
{
	return baroAlt;
}



