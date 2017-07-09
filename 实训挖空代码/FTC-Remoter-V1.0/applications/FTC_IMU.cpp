/******************** (C) COPYRIGHT 2015 FTC *******************************
 * 作者		 ：FTC
 * 文件名  ：FTC_IMU.cpp
 * 描述    ：飞行器姿态计算
**********************************************************************************/
#include "FTC_IMU.h"

FTC_IMU imu;

FTC_IMU::FTC_IMU()
{
}

//IMU初始化
void FTC_IMU::Init()
{
	//滤波器参数初始化
	filter_Init();
	//传感器初始化
	sensor_Init();	
}

//更新传感器数据
void FTC_IMU::updateSensor()
{
	//读取加速度
	mpu6050.Read_Acc_Data();
	//读取角速度
	mpu6050.Read_Gyro_Data();	
	//获取角速度，单位为度每秒
	Gyro = mpu6050.Get_Gyro_in_dps();
	//获取加速度采样值
	Acc = mpu6050.Get_Acc();
}


//计算飞行器姿态
void FTC_IMU::getAttitude()
{
	//加速度数据一阶低通滤波
	Acc_lpf = LPF_1st(Acc_lpf, Acc, ftc.factor.acc_lpf);

	DCM_CF(Gyro,Acc_lpf,IMU_LOOP_TIME*1e-6);
}


//余弦矩阵更新姿态
void FTC_IMU::DCM_CF(Vector3f gyro,Vector3f acc, float deltaT)
{
}

void FTC_IMU::filter_Init()
{
	//加速度一阶低通滤波器系数计算
	ftc.factor.acc_lpf = LPF_1st_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT);
	
	//互补滤波器系数计算
	ftc.factor.gyro_cf = CF_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);	
}

void FTC_IMU::sensor_Init()
{
	//初始化MPU6050，1Khz采样率，98Hz低通滤波
	mpu6050.Init(1000,98);
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
