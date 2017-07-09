/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_DT.cpp
 * 描述    ：无线数据收发和处理
**********************************************************************************/
#include "FTC_DT.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

FTC_DT dt;

void FTC_DT::Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	u8 nohead = 1;//1为开启无头模式
	int32_t pitch, roll;
	
	for(u8 i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	ftc.f.FAILSAFE = 0;
/////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
			mpu6050.Acc_CALIBRATED = 1;
		if(*(data_buf+4)==0X02)
			mpu6050.Gyro_CALIBRATED = 1;
		if(*(data_buf+4)==0X03)
		{
			mpu6050.Acc_CALIBRATED = 1;		
			mpu6050.Gyro_CALIBRATED = 1;			
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
 			f.send_pid1 = 1;
			f.send_pid2 = 1;
			f.send_pid3 = 1;
			f.send_pid4 = 1;
			f.send_pid5 = 1;
			f.send_pid6 = 1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		//读取版本信息
		{
			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//恢复默认参数
		{
			fc.PID_Reset();
			param.SAVE_PID();
		}
	}

	if(*(data_buf+2)==0X03)
	{
		rc.rawData[THROTTLE] = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		rc.rawData[YAW] = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		rc.rawData[ROLL] = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		rc.rawData[PITCH] = (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		rc.rawData[AUX1] = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		rc.rawData[AUX2] = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		rc.rawData[AUX3] = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		rc.rawData[AUX4] = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
		
		if(nohead)//无头模式
		{
			roll = rc.rawData[ROLL] - RC_MID;
			pitch = rc.rawData[PITCH] - RC_MID;
			rc.rawData[ROLL] = RC_MID - pitch * sinf(radians(imu.angle.z)) + roll * cosf(radians(imu.angle.z));
			rc.rawData[PITCH] = RC_MID + pitch * cosf(radians(imu.angle.z)) + roll * sinf(radians(imu.angle.z));
		}
	}

	if(*(data_buf+2)==0X10)								//PID1
	{
		fc.pid[PIDROLL].kP = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5)) / 1000;
		fc.pid[PIDROLL].kI = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7)) / 1000;
		fc.pid[PIDROLL].kD = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9)) / 1000;
		fc.pid[PIDPITCH].kP = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11)) / 1000;
		fc.pid[PIDPITCH].kI = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13)) / 1000;
		fc.pid[PIDPITCH].kD = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15)) / 1000;
		fc.pid[PIDYAW].kP = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17)) / 1000;
		fc.pid[PIDYAW].kI = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19)) / 1000;
		fc.pid[PIDYAW].kD = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21)) / 1000;
		Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X11)								//PID2
	{
		fc.pid[PIDANGLE].kP = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5)) / 1000;
		fc.pid[PIDANGLE].kI = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7)) / 1000;
		fc.pid[PIDANGLE].kD = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9)) / 1000;
		fc.pid[PIDMAG].kP = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11)) / 1000;
		fc.pid[PIDMAG].kI = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13)) / 1000;
		fc.pid[PIDMAG].kD = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15)) / 1000;
		fc.pid[PIDVELZ].kP = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17)) / 1000;
		fc.pid[PIDVELZ].kI = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19)) / 1000;
		fc.pid[PIDVELZ].kD = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21)) / 1000;
		Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X12)								//PID3
	{
		fc.pid[PIDALT].kP = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5)) / 1000;
		fc.pid[PIDALT].kI = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7)) / 1000;
		fc.pid[PIDALT].kD = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9)) / 1000;
		Send_Check(*(data_buf+2),sum);
		param.SAVE_PID();
	}
	if(*(data_buf+2)==0X13)								//PID4
	{
		Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		Send_Check(*(data_buf+2),sum);

	}
	
}

void FTC_DT::Check_Event(void)
{
	
#ifdef FTC_DT_USE_NRF24l01	
	nrf.Check_Event();
#endif
	
}

void FTC_DT::Data_Exchange(void)
{
	static u8 cnt = 0;
	static u8 senser_cnt 	= 10;
	static u8 user_cnt 	  = 10;
	static u8 status_cnt 	= 15;
	static u8 rcdata_cnt 	= 50;
	static u8 motopwm_cnt	= 50;
	static u8 power_cnt		=	50;
	static u8 senser2_cnt = 50;
	
	if((cnt % senser_cnt) == (senser_cnt-1))
		f.send_senser = 1;	
	if((cnt % senser2_cnt) == (senser2_cnt-1))
		f.send_senser2 = 1;	
	
	if((cnt % user_cnt) == (user_cnt-1))
		f.send_user = 1;
	
	if((cnt % status_cnt) == (status_cnt-1))
		f.send_status = 1;	
	
	if((cnt % rcdata_cnt) == (rcdata_cnt-1))
		f.send_rcdata = 1;	
	
	if((cnt % motopwm_cnt) == (motopwm_cnt-1))
		f.send_motopwm = 1;	
	
	if((cnt % power_cnt) == (power_cnt-1))
		f.send_power = 1;		
	
	cnt++;
/////////////////////////////////////////////////////////////////////////////////////
	if(f.send_version)
	{
		f.send_version = 0;
		Send_Version(1,300,110,400,0);
	}
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_status)
	{
		f.send_status = 0;
		Send_Status(imu.angle.x,imu.angle.y,imu.angle.z,nav.position.z,ftc.f.ALTHOLD,ftc.f.ARMED);	
	}	
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_user)
	{
		f.send_user = 0;
		//Send_User();
	}
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_senser)
	{
		f.send_senser = 0;
		Send_Senser(imu.Acc.x,imu.Acc.y,imu.Acc.z,
								imu.Gyro.x,imu.Gyro.y,imu.Gyro.z,
								0,0,0);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_senser2)
	{
		f.send_senser2 = 0;
		Send_Senser2(nav.baroAlt * 100, nav.ultraAlt * 100);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_rcdata)
	{
		f.send_rcdata = 0;
		Send_RCData(rc.rawData[THROTTLE],rc.rawData[YAW],rc.rawData[ROLL],rc.rawData[PITCH],
								rc.rawData[AUX1],rc.rawData[AUX2],rc.rawData[AUX3],
								rc.rawData[AUX4],rc.rawData[AUX5],rc.rawData[AUX6]);
	}	
/////////////////////////////////////////////////////////////////////////////////////	
	else if(f.send_motopwm)
	{
		f.send_motopwm = 0;
		int16_t Moto_PWM[6];
		motor.getPWM(Moto_PWM);
		for(u8 i=0;i<6;i++)
			Moto_PWM[i] -= 1000;

		//Send_MotoPWM(Moto_PWM[0], Moto_PWM[1], Moto_PWM[2], Moto_PWM[3], Moto_PWM[4], Moto_PWM[5],0,0);
		Send_MotoPWM(888, 888, 888, 888, 888, 888, 888, 888);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_power)
	{
		f.send_power = 0;
		Send_Power(V_Battery * 100, 0);
	}
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_pid1)
	{
		f.send_pid1 = 0;
		Send_PID(1,fc.pid[PIDROLL].kP,fc.pid[PIDROLL].kI,fc.pid[PIDROLL].kD,
							 fc.pid[PIDPITCH].kP,fc.pid[PIDPITCH].kI,fc.pid[PIDPITCH].kD,
							 fc.pid[PIDYAW].kP,fc.pid[PIDYAW].kI,fc.pid[PIDYAW].kD);
	}	
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_pid2)
	{
		f.send_pid2 = 0;
		Send_PID(2,fc.pid[PIDANGLE].kP,fc.pid[PIDANGLE].kI,fc.pid[PIDANGLE].kD,
							 fc.pid[PIDMAG].kP,fc.pid[PIDMAG].kI,fc.pid[PIDMAG].kD,
							 fc.pid[PIDVELZ].kP,fc.pid[PIDVELZ].kI,fc.pid[PIDVELZ].kD);
	}
/////////////////////////////////////////////////////////////////////////////////////
	else if(f.send_pid3)
	{
		f.send_pid3 = 0;
		Send_PID(3,fc.pid[PIDALT].kP,fc.pid[PIDALT].kI,fc.pid[PIDALT].kD,
							 0,0,0,
							 0,0,0);
	}
}

void FTC_DT::Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver)
{
	u8 _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=hardware_type;
	data_to_send[_cnt++]=BYTE1(hardware_ver);
	data_to_send[_cnt++]=BYTE0(hardware_ver);
	data_to_send[_cnt++]=BYTE1(software_ver);
	data_to_send[_cnt++]=BYTE0(software_ver);
	data_to_send[_cnt++]=BYTE1(protocol_ver);
	data_to_send[_cnt++]=BYTE0(protocol_ver);
	data_to_send[_cnt++]=BYTE1(bootloader_ver);
	data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}
void FTC_DT::Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = alt;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}


void FTC_DT::Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	u8 _cnt=0;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	Send_Data(data_to_send, _cnt);
}
void FTC_DT::Send_Senser2(s32 alt_bar,u16 alt_csb)
{
	u8 _cnt=0;
	vs32 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;
	
	_temp = alt_bar;
	data_to_send[_cnt++]=BYTE3(_temp);
	data_to_send[_cnt++]=BYTE2(_temp);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = alt_csb;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	Send_Data(data_to_send, _cnt);
}

void FTC_DT::Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}
void FTC_DT::Send_Power(u16 votage, u16 current)
{
	u8 _cnt=0;
	u16 temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = current;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}
void FTC_DT::Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
	u8 _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}

void FTC_DT::Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 _cnt=0;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	Send_Data(data_to_send, _cnt);
}

void FTC_DT::Send_Check(u8 head, u8 check_sum)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	u8 sum = 0;
	for(u8 i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	Send_Data(data_to_send, 8);
}

void FTC_DT::Send_Data(u8 *dataToSend , u8 length)
{
	
#ifdef FTC_DT_USE_Bluetooth
	Uart1_Put_Buf(data_to_send,length);
#endif
	
#ifdef FTC_DT_USE_NRF24l01
	nrf.TxPacket(data_to_send,length);
#endif
}


void FTC_DT::Failsafe_Check(void)
{
		static u8 failsafeCnt = 0;
		if(failsafeCnt > 30)
		{
			failsafeCnt = 0;
			if(!ftc.f.FAILSAFE)
				ftc.f.FAILSAFE = 1;
			else
			{	
				ftc.f.ARMED = 0;
			}
		}
		failsafeCnt++;	
}




/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
