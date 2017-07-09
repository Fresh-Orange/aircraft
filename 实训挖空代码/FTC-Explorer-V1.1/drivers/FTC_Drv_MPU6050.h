#ifndef __FTC_DRV_MPU6050_H
#define	__FTC_DRV_MPU6050_H

#include "board.h"

#define CALIBRATING_GYRO_CYCLES             200
#define CALIBRATING_ACC_CYCLES              200

class FTC_MPU6050:public FTC_I2C_Soft
{
public:
	
	FTC_MPU6050(void);

	u8 Acc_CALIBRATED;
	u8 Gyro_CALIBRATED;
	Vector3i Acc_Offset,Gyro_Offset;

	//初始化6050
	void Init(uint16_t sample_rate, uint16_t lpf);
	//读取加速度
	void Read_Acc_Data(void);
	//读取角速度
	void Read_Gyro_Data(void);
	//返回加速度的值
	Vector3f Get_Acc(void);
	//返回角速度的值
	Vector3f Get_Gyro(void);
	//返回单位为度每秒的角速度
	Vector3f Get_Gyro_in_dps(Vector3f GyroRaw);

private:
	
	u8 mpu6050_buffer[14]; //接收数据缓存区
	Vector3f Acc_ADC,Gyro_ADC;
	Vector3f Gyro_dps;

	//加速度零偏矫正
	void CalOffset_Acc(void);
	//陀螺仪零偏矫正
	void CalOffset_Gyro(void);

	void delayms(uint16_t ms);
};

extern FTC_MPU6050 mpu6050;

#endif








