/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：ANO_Config.cpp
 * 描述    ：配置文件
**********************************************************************************/

#include "FTC_Config.h"

FTC_Config ftc;


FTC_Config::FTC_Config(void)
{
	f.ANGLE_MODE = 1;
}

//指示灯
void FTC_Config::Pilot_Light(void)
{
	static u8 cnt = 0,cnt2 = 0;
	
	if(f.LOWPOWER)
	{
		cnt++;
		switch(cnt)
		{
			case 1:
				led.ON1();
				led.ON2();
				break;
			case 5:
				led.OFF1();
				led.OFF2();
				break;
			case 10:
				cnt = 0;
				break;			
		}		
	}
	else
	{
		if(f.ARMED && !f.ALTHOLD)
		{
			cnt++;
			switch(cnt)
			{
				case 1:
					led.ON1();
					break;
				case 20:
					led.OFF1();
					break;
				case 40:
					cnt = 0;
					break;			
			}
		}
		else if(f.ARMED && f.ALTHOLD)
		{
			cnt2++;		
			switch(cnt2)
			{
				case 1:
					led.ON1();
					led.OFF2();
					break;
				case 20:
					led.OFF1();
					led.ON2();
					break;
				case 40:
					cnt2 = 0;
					break;			
			}
		}
		else
		{
			led.ON1();
			led.ON2();
		}
	}
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
