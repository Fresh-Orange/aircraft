#ifndef __FTC_DT_H
#define __FTC_DT_H

#include "FTC_Config.h"

class FTC_DT
{
	
public:
	
	//����Ƿ��н��յ���������
	void Check_Event(void);
	//���ݷ���
	void Data_Exchange(void);
	//ʧ�ر������
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









