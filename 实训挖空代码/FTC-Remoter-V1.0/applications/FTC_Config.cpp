/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Config.cpp
 * 描述    ：配置文件
**********************************************************************************/

#include "FTC_Config.h"

FTC_Config ftc;


FTC_Config::FTC_Config(void)
{
	f.STICKMODE = 1;
}

//指示灯
void FTC_Config::Pilot_Light(void)
{
	static u8 cnt = 0;
	
	if(f.ACCELMODE)
	{
		cnt++;
		switch(cnt)
		{
			case 1:
				led.ON();
				break;
			case 25:
				led.OFF();
				break;	
			case 50:
				cnt = 0;
				break;
		}
	}
	else
	{
		led.ON(); 
	}
	
}
/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
