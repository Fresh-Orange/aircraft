/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Param.cpp
 * 描述    ：参数读取和保存
**********************************************************************************/
#include "FTC_Param.h"
#include "board.h"

FTC_Param param;

#define EE_RcDataTrimROLL	1
#define EE_RcDataTrimPITCH	2
#define EE_RcDataTrimYAW	3
#define EE_RcDataTrimTHROTTLE	4



void FTC_Param::Init(void)
{	
	READ_CONF();	
}

void FTC_Param::SAVE_RC_OFFSET(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_RcDataTrimROLL], rc.DataTrim[ROLL]);
	EE_WriteVariable(VirtAddVarTab[EE_RcDataTrimPITCH], rc.DataTrim[PITCH]);
	EE_WriteVariable(VirtAddVarTab[EE_RcDataTrimYAW], rc.DataTrim[YAW]);
	EE_WriteVariable(VirtAddVarTab[EE_RcDataTrimTHROTTLE], rc.DataTrim[THROTTLE]);
}
void FTC_Param::READ_RC_OFFSET(void)
{
	uint16_t temp[4];
	EE_ReadVariable(VirtAddVarTab[EE_RcDataTrimROLL], &temp[0]);
	EE_ReadVariable(VirtAddVarTab[EE_RcDataTrimPITCH], &temp[1]);
	EE_ReadVariable(VirtAddVarTab[EE_RcDataTrimYAW], &temp[2]);
	EE_ReadVariable(VirtAddVarTab[EE_RcDataTrimTHROTTLE], &temp[3]);
	
	for(u8 i=0;i<4;i++)
		rc.DataTrim[i] = temp[i];
}

void FTC_Param::READ_CONF(void)
{
	READ_RC_OFFSET();
}










