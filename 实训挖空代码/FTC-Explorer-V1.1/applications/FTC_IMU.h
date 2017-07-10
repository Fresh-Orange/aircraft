#ifndef __FTC_IMU_H
#define __FTC_IMU_H

#include "FTC_Config.h"

class FTC_IMU : public FTC_Filter
{
	
public:

	FTC_IMU();
	
	//ŷ���Ǳ�ʾ�ķ�������̬
	Vector3f angle;
	
	Vector3f Gyro, Gyro_lpf, Acc, Acc_lpf; 

	LPF2ndData_t Acc_lpf_2nd, Gyro_lpf_2nd;

	LPF4thData_t Acc_lpf_4th, Gyro_lpf_4th;

	float magHold;
	
	void Init();
	
	//���´���������
	void updateSensor();	
	
	//�����������̬
	void getAttitude();
	
	Vector3f Get_Accel_Ef();
		
	
private:

	Quaternion Q;

	int32_t accRatio;

	float getDeltaT(uint32_t time);

	//�������Ҿ���ͻ����˲�����̬����
	void DCM_CF(Vector3f gyro,Vector3f acc, float deltaT);
	//������Ԫ���ͻ����˲�����̬����
	void Quaternion_CF(Vector3f gyro,Vector3f acc, float deltaT);

	//�˲���������ʼ��
	void filter_Init();
	//��������ʼ��
	void sensor_Init();

};

extern FTC_IMU imu;

#endif
