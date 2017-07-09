/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FtcCopter.cpp
 * 描述    ：Filter创新探索者V1.0微型六轴飞行器
 * 代码版本：V1.0
 * 时间		 ：2015/12/1
**********************************************************************************/
#include "FTC_Config.h"

int main(void)
{
	//初始化飞控板的硬件设置
	FTC_Hexacopter_board_Init();
	
	//初始化参数
	param.Init();
	
	//初始化IMU（惯性测量单元）
	imu.Init();	
	
	nav.Init();
	
	while(1)
	{
		FTC_Loop();
	}
	
	return 0;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
