/******************** (C) COPYRIGHT 2015 FTC *******************************
 * ����		 ��FTC
 * �ļ���  ��FTC_IMU.cpp
 * ����    ����������̬����
**********************************************************************************/
#include "FTC_IMU.h"

FTC_IMU imu;

FTC_IMU::FTC_IMU()
{
}

//IMU��ʼ��
void FTC_IMU::Init()
{
	//�˲���������ʼ��
	filter_Init();
	//��������ʼ��
	sensor_Init();	
}

//���´���������
void FTC_IMU::updateSensor()
{
	//��ȡ���ٶ�
	mpu6050.Read_Acc_Data();
	//��ȡ���ٶ�
	mpu6050.Read_Gyro_Data();	
	//��ȡ���ٶȣ���λΪ��ÿ��
	Gyro = mpu6050.Get_Gyro_in_dps();
	//��ȡ���ٶȲ���ֵ
	Acc = mpu6050.Get_Acc();
}


//�����������̬
void FTC_IMU::getAttitude()
{
	//���ٶ�����һ�׵�ͨ�˲�
	Acc_lpf = LPF_1st(Acc_lpf, Acc, ftc.factor.acc_lpf);

	DCM_CF(Gyro,Acc_lpf,IMU_LOOP_TIME*1e-6);
}


//���Ҿ��������̬
void FTC_IMU::DCM_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
}

void FTC_IMU::filter_Init()
{
	//���ٶ�һ�׵�ͨ�˲���ϵ������
	ftc.factor.acc_lpf = LPF_1st_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT);
	
	//�����˲���ϵ������
	ftc.factor.gyro_cf = CF_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);	
}

void FTC_IMU::sensor_Init()
{
	//��ʼ��MPU6050��1Khz�����ʣ�98Hz��ͨ�˲�
	mpu6050.Init(1000,98);
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
