#ifndef __FTC_DT_H
#define __FTC_DT_H

#include "FTC_Config.h"

class FTC_DT
{
	
public:
	
	//检查是否有接收到无线数据
	void Check_Event(void);
	//数据发送
	void Data_Exchange(void);
	//失控保护检查
	void Failsafe_Check(void);

	class flag{
		public:
		u8 Send_RCData;
	}f;
	
	void Send_Data(u8 *dataToSend , u8 length);	
	
private:
		
	u8 data_to_send[50];

	void Send_RCData(void);



};

extern FTC_DT dt;

#endif









