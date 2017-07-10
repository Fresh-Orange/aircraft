#ifndef __FTC_IMU_H
#define __FTC_IMU_H

#include "FTC_Config.h"

class FTC_IMU : public FTC_Filter
{
	
public:

	FTC_IMU();
	
	//ŷ���Ǳ�ʾ�ķ�������̬
	Vector3f angle;
	
	Vector3f Gyro, Acc, Acc_lpf; 
	
	void Init();
	
	//���´���������
	void updateSensor();	
	
	//�����������̬
	void getAttitude();
	
private:

	//�������Ҿ���ͻ����˲�����̬����
	void DCM_CF(Vector3f gyro,Vector3f acc, float deltaT);

	//�˲���������ʼ��
	void filter_Init();
	//��������ʼ��
	void sensor_Init();

};

extern FTC_IMU imu;

#endif

