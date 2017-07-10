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
	Gyro = mpu6050.Get_Gyro();
	//��ȡ���ٶȲ���ֵ
	Acc = mpu6050.Get_Acc();
}


//�����������̬
void FTC_IMU::getAttitude()
{
	float deltaT;
	Vector3d accTemp, gyroTemp;
	
#ifdef FTC_IMU_USE_LPF_1st	
	//���ٶ�����һ�׵�ͨ�˲�
	Acc_lpf = LPF_1st(Acc_lpf, Acc, ftc.factor.acc_lpf);
#endif	
	
#ifdef FTC_IMU_USE_LPF_2nd	
	//���ٶ����ݶ��׵�ͨ�˲�
	Acc_lpf = LPF_2nd(&Acc_lpf_2nd, Acc);
#endif
	
	//���������ݶ��׵�ͨ�˲�
	Gyro_lpf = LPF_2nd(&Gyro_lpf_2nd, Gyro);
	
#ifdef FTC_IMU_USE_LPF_4th
	//���ٶ������Ľ׵�ͨ�˲�
	accTemp(double(Acc.x), double(Acc.y), double(Acc.z));
	accTemp = LPF_Butterworth_4th(accTemp, &Acc_lpf_4th);
	Acc_lpf(float(accTemp.x),float(accTemp.y),float(accTemp.z));
#endif
	

	//����ʵ�ʲ����ļ��ٶȺ��������ٶȵı�ֵ
	accRatio = Acc_lpf.length_squared() * 100 / (ACC_1G * ACC_1G);		
	
	deltaT = getDeltaT(GetSysTime_us());
	
#ifdef FTC_IMU_USE_DCM_CF
	DCM_CF(mpu6050.Get_Gyro_in_dps(Gyro_lpf),Acc_lpf,deltaT);
#endif
#ifdef FTC_IMU_USE_Quaternions_CF
	Quaternion_CF(mpu6050.Get_Gyro_in_dps(Gyro_lpf),Acc_lpf,deltaT);
#endif
}

//��ȡ�������ļ��ٶ��ڵ�������ϵ��ͶӰ
Vector3f FTC_IMU::Get_Accel_Ef(void)
{
	Matrix3f dcm;
	Vector3f anglerad;
	
	//��̬��ת����
	anglerad(-radians(angle.x), -radians(angle.y), radians(angle.z));	//ûд��z�����Ϊ+���ǶԵ�

	//�����ʾ��ת�����Ҿ���
	dcm.from_euler(anglerad);
	
	return dcm * Acc_lpf;
}

//���Ҿ��������̬
void FTC_IMU::DCM_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
	//acc.get_rollpitch(angle);
	//acc.get_yaw(angle);
	
	static Vector3f last_gyro;
	static Vector3f angleNow;
	angleNow = (gyro + last_gyro)*0.5*deltaT;
	last_gyro = gyro;
	
	Matrix3<float> dcm;
	dcm.from_euler(angleNow);
	static Vector3f g(0,0,ACC_1G);
	static Vector3f m(ACC_1G,0,0);
	
	g = dcm * g;
	m = dcm * m;
	FTC_Filter fi;
	g = fi.CF_1st(g, acc, ftc.factor.gyro_cf);
	//m = fi.CF_1st(m, acc, ftc.factor.gyro_cf);
	
	g.get_rollpitch(angle);
	m.get_yaw(angle);
	
}

#define Kp 2.0f        //���ٶ�Ȩ�أ�Խ��������ٶȲ���ֵ����Խ��
#define Ki 0.001f      //����������
//��Ԫ��������̬
void FTC_IMU::Quaternion_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
	//to do
}

void FTC_IMU::filter_Init()
{
	//���ٶ�һ�׵�ͨ�˲���ϵ������
	ftc.factor.acc_lpf = LPF_1st_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT);
	
	//���ٶȶ��׵�ͨ�˲���ϵ������
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT, &Acc_lpf_2nd);
	
	//�����Ƕ��׵�ͨ�˲���ϵ������	
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_LPF_CUT, &Gyro_lpf_2nd);	
	
	//�����˲���ϵ������
	ftc.factor.gyro_cf = CF_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);	
}

void FTC_IMU::sensor_Init()
{
	//��ʼ��MPU6050��1Khz�����ʣ�98Hz��ͨ�˲�
	mpu6050.Init(1000,98);
}

float FTC_IMU::getDeltaT(uint32_t currentT)
{
	static uint32_t previousT;
	float	deltaT = (currentT - previousT) * 1e-6;	
	previousT = currentT;
	
	return deltaT;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
