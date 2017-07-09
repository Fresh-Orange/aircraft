/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_MPU6050.cpp
 * 描述    ：MPU6050
**********************************************************************************/

#include "FTC_Drv_MPU6050.h"

// MPU6050, 硬件I2c地址 0x68，模拟i2c地址0xD0
#define MPU6050_ADDRESS         0xD0	// 0x68
#define DMP_MEM_START_ADDR 0x6E
#define DMP_MEM_R_W 0x6F

#define MPU_RA_XG_OFFS_TC       0x00    //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_YG_OFFS_TC       0x01    //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_ZG_OFFS_TC       0x02    //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_X_FINE_GAIN      0x03    //[7:0] X_FINE_GAIN
#define MPU_RA_Y_FINE_GAIN      0x04    //[7:0] Y_FINE_GAIN
#define MPU_RA_Z_FINE_GAIN      0x05    //[7:0] Z_FINE_GAIN
#define MPU_RA_XA_OFFS_H        0x06    //[15:0] XA_OFFS
#define MPU_RA_XA_OFFS_L_TC     0x07
#define MPU_RA_YA_OFFS_H        0x08    //[15:0] YA_OFFS
#define MPU_RA_YA_OFFS_L_TC     0x09
#define MPU_RA_ZA_OFFS_H        0x0A    //[15:0] ZA_OFFS
#define MPU_RA_ZA_OFFS_L_TC     0x0B
#define MPU_RA_PRODUCT_ID       0x0C    // Product ID Register
#define MPU_RA_XG_OFFS_USRH     0x13    //[15:0] XG_OFFS_USR
#define MPU_RA_XG_OFFS_USRL     0x14
#define MPU_RA_YG_OFFS_USRH     0x15    //[15:0] YG_OFFS_USR
#define MPU_RA_YG_OFFS_USRL     0x16
#define MPU_RA_ZG_OFFS_USRH     0x17    //[15:0] ZG_OFFS_USR
#define MPU_RA_ZG_OFFS_USRL     0x18
#define MPU_RA_SMPLRT_DIV       0x19
#define MPU_RA_CONFIG           0x1A
#define MPU_RA_GYRO_CONFIG      0x1B
#define MPU_RA_ACCEL_CONFIG     0x1C
#define MPU_RA_FF_THR           0x1D
#define MPU_RA_FF_DUR           0x1E
#define MPU_RA_MOT_THR          0x1F
#define MPU_RA_MOT_DUR          0x20
#define MPU_RA_ZRMOT_THR        0x21
#define MPU_RA_ZRMOT_DUR        0x22
#define MPU_RA_FIFO_EN          0x23
#define MPU_RA_I2C_MST_CTRL     0x24
#define MPU_RA_I2C_SLV0_ADDR    0x25
#define MPU_RA_I2C_SLV0_REG     0x26
#define MPU_RA_I2C_SLV0_CTRL    0x27
#define MPU_RA_I2C_SLV1_ADDR    0x28
#define MPU_RA_I2C_SLV1_REG     0x29
#define MPU_RA_I2C_SLV1_CTRL    0x2A
#define MPU_RA_I2C_SLV2_ADDR    0x2B
#define MPU_RA_I2C_SLV2_REG     0x2C
#define MPU_RA_I2C_SLV2_CTRL    0x2D
#define MPU_RA_I2C_SLV3_ADDR    0x2E
#define MPU_RA_I2C_SLV3_REG     0x2F
#define MPU_RA_I2C_SLV3_CTRL    0x30
#define MPU_RA_I2C_SLV4_ADDR    0x31
#define MPU_RA_I2C_SLV4_REG     0x32
#define MPU_RA_I2C_SLV4_DO      0x33
#define MPU_RA_I2C_SLV4_CTRL    0x34
#define MPU_RA_I2C_SLV4_DI      0x35
#define MPU_RA_I2C_MST_STATUS   0x36
#define MPU_RA_INT_PIN_CFG      0x37
#define MPU_RA_INT_ENABLE       0x38
#define MPU_RA_DMP_INT_STATUS   0x39
#define MPU_RA_INT_STATUS       0x3A
#define MPU_RA_ACCEL_XOUT_H     0x3B
#define MPU_RA_ACCEL_XOUT_L     0x3C
#define MPU_RA_ACCEL_YOUT_H     0x3D
#define MPU_RA_ACCEL_YOUT_L     0x3E
#define MPU_RA_ACCEL_ZOUT_H     0x3F
#define MPU_RA_ACCEL_ZOUT_L     0x40
#define MPU_RA_TEMP_OUT_H       0x41
#define MPU_RA_TEMP_OUT_L       0x42
#define MPU_RA_GYRO_XOUT_H      0x43
#define MPU_RA_GYRO_XOUT_L      0x44
#define MPU_RA_GYRO_YOUT_H      0x45
#define MPU_RA_GYRO_YOUT_L      0x46
#define MPU_RA_GYRO_ZOUT_H      0x47
#define MPU_RA_GYRO_ZOUT_L      0x48
#define MPU_RA_EXT_SENS_DATA_00 0x49
#define MPU_RA_MOT_DETECT_STATUS    0x61
#define MPU_RA_I2C_SLV0_DO      0x63
#define MPU_RA_I2C_SLV1_DO      0x64
#define MPU_RA_I2C_SLV2_DO      0x65
#define MPU_RA_I2C_SLV3_DO      0x66
#define MPU_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU_RA_SIGNAL_PATH_RESET    0x68
#define MPU_RA_MOT_DETECT_CTRL      0x69
#define MPU_RA_USER_CTRL        0x6A
#define MPU_RA_PWR_MGMT_1       0x6B
#define MPU_RA_PWR_MGMT_2       0x6C
#define MPU_RA_BANK_SEL         0x6D
#define MPU_RA_MEM_START_ADDR   0x6E
#define MPU_RA_MEM_R_W          0x6F
#define MPU_RA_DMP_CFG_1        0x70
#define MPU_RA_DMP_CFG_2        0x71
#define MPU_RA_FIFO_COUNTH      0x72
#define MPU_RA_FIFO_COUNTL      0x73
#define MPU_RA_FIFO_R_W         0x74
#define MPU_RA_WHO_AM_I         0x75

#define MPU6050_SMPLRT_DIV      0       // 8000Hz

#define MPU6050_LPF_256HZ       0
#define MPU6050_LPF_188HZ       1
#define MPU6050_LPF_98HZ        2
#define MPU6050_LPF_42HZ        3
#define MPU6050_LPF_20HZ        4
#define MPU6050_LPF_10HZ        5
#define MPU6050_LPF_5HZ         6

#define MPU6050A_2mg                ((float)0.00006103f)  // 0.00006250 g/LSB
#define MPU6050A_4mg                ((float)0.00012207f)  // 0.00012500 g/LSB
#define MPU6050A_8mg                ((float)0.00024414f)  // 0.00025000 g/LSB

#define MPU6050G_s250dps            ((float)0.0076335f)  // 0.0087500 dps/LSB
#define MPU6050G_s500dps            ((float)0.0152671f)  // 0.0175000 dps/LSB
#define MPU6050G_s2000dps           ((float)0.0609756f)  // 0.0700000 dps/LSB

FTC_MPU6050 mpu6050;

FTC_MPU6050::FTC_MPU6050()
{
	
}

void FTC_MPU6050::delayms(uint16_t ms){
	uint16_t i = 200;
	for(u8 j=0;j<ms;j++)
		while(i--);
}

//MPU6050初始化，传入参数：采样率，低通滤波频率
void FTC_MPU6050::Init(uint16_t sample_rate, uint16_t lpf)
{
	uint8_t default_filter;
	
	switch (lpf) {
	case 5:
			default_filter = MPU6050_LPF_5HZ;
			break;
	case 10:
			default_filter = MPU6050_LPF_10HZ;
			break;
	case 20:
			default_filter = MPU6050_LPF_20HZ;
			break;
	case 42:
			default_filter = MPU6050_LPF_42HZ;
			break;
	case 98:
			default_filter = MPU6050_LPF_98HZ;
			break;
	case 188:
			default_filter = MPU6050_LPF_188HZ;
			break;
	case 256:
			default_filter = MPU6050_LPF_256HZ;
			break;
	default:
			default_filter = MPU6050_LPF_98HZ;
			break;
	}	
	
	//设备复位
	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_PWR_MGMT_1, 0x80);	
	
	delayms(5);
	
	//陀螺仪采样率，0x00(1000Hz)   采样率 = 陀螺仪的输出率 / (1 + SMPLRT_DIV)
	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));	
	//设置设备时钟源，陀螺仪Z轴
	I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, 0x03);	
	//i2c旁路模式
	I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0); 
	// INT_PIN_CFG   -- INT_LEVEL_HIGH, INT_OPEN_DIS, LATCH_INT_DIS, INT_RD_CLEAR_DIS, FSYNC_INT_LEVEL_HIGH, FSYNC_INT_DIS, I2C_BYPASS_EN, CLOCK_DIS
	//低通滤波频率，0x03(42Hz)
	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_CONFIG, default_filter);	
	 //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_GYRO_CONFIG, 0x18); 
	//加速计自检、测量范围(不自检，+-8G)			
	I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_ACCEL_CONFIG, 2 << 3);	
}

//读取加速度
void FTC_MPU6050::Read_Acc_Data(void)
{
	int16_t acc_temp[3];
	
	mpu6050_buffer[0] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_L); 
	mpu6050_buffer[1] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H);
	acc_temp[0] = ((((int16_t)mpu6050_buffer[1]) << 8) | mpu6050_buffer[0])- Acc_Offset.x;  //加速度X轴

	mpu6050_buffer[2] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_L);
	mpu6050_buffer[3] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_H);
	acc_temp[1] = ((((int16_t)mpu6050_buffer[3]) << 8) | mpu6050_buffer[2])- Acc_Offset.y;  //加速度Y轴

	mpu6050_buffer[4] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_L);
	mpu6050_buffer[5] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_H);
	acc_temp[2] = ((((int16_t)mpu6050_buffer[5]) << 8) | mpu6050_buffer[4])- Acc_Offset.z;  //加速度Z轴
	
	Acc_ADC((float)acc_temp[0],(float)acc_temp[1],(float)acc_temp[2]);
	
	CalOffset_Acc();
}

//读取角速度
void FTC_MPU6050::Read_Gyro_Data(void)
{
	int16_t gyro_temp[3];
	
	mpu6050_buffer[6] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_L); 
	mpu6050_buffer[7] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_H);
	gyro_temp[0] = ((((int16_t)mpu6050_buffer[7]) << 8) | mpu6050_buffer[6])- Gyro_Offset.x;	//陀螺仪X轴

	mpu6050_buffer[8] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_L);
	mpu6050_buffer[9] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_H);
	gyro_temp[1] = ((((int16_t)mpu6050_buffer[9]) << 8) | mpu6050_buffer[8])- Gyro_Offset.y;	//陀螺仪Y轴

	mpu6050_buffer[10] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_L);
	mpu6050_buffer[11] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_H);
	gyro_temp[2] = ((((int16_t)mpu6050_buffer[11]) << 8) | mpu6050_buffer[10])- Gyro_Offset.z;	  //陀螺仪Z轴		
	
	Gyro_ADC((float)gyro_temp[0], (float)gyro_temp[1], (float)gyro_temp[2]);
	
	CalOffset_Gyro();
}

Vector3f FTC_MPU6050::Get_Acc(void)
{
 return Acc_ADC;	
}

Vector3f FTC_MPU6050::Get_Gyro(void)
{
 return Gyro_ADC;		
}

Vector3f FTC_MPU6050::Get_Gyro_in_dps(void)
{
	Gyro_dps.x = radians(Gyro_ADC.x * MPU6050G_s2000dps);   // dps
	Gyro_dps.y = radians(Gyro_ADC.y * MPU6050G_s2000dps);   // dps
	Gyro_dps.z = radians(Gyro_ADC.z * MPU6050G_s2000dps);   // dps	
	return Gyro_dps;
}

//加速度零偏矫正
void FTC_MPU6050::CalOffset_Acc(void)
{
	if(Acc_CALIBRATED)
		{
			static Vector3f	tempAcc;
			static uint16_t cnt_a=0;

			if(cnt_a==0)
			{
				Acc_Offset(0, 0, 0);
				tempAcc(0, 0, 0);
				cnt_a = 1;
				return;
			}			
			tempAcc += Acc_ADC;
			if(cnt_a == CALIBRATING_ACC_CYCLES)
			{
				Acc_Offset.x = tempAcc.x/cnt_a;
				Acc_Offset.y = tempAcc.y/cnt_a;
				Acc_Offset.z = tempAcc.z/cnt_a - ACC_1G;
				cnt_a = 0;
				Acc_CALIBRATED = 0;
				//param.SAVE_ACC_OFFSET();//保存数据
				return;
			}
			cnt_a++;		
		}	
	
}

//陀螺仪零偏矫正
void FTC_MPU6050::CalOffset_Gyro(void)
{
	if(Gyro_CALIBRATED)
	{
		static Vector3f	tempGyro;
		static uint16_t cnt_g=0;
		if(cnt_g==0)
		{
			Gyro_Offset(0, 0, 0);
			tempGyro(0, 0, 0);
			cnt_g = 1;
			return;
		}
		tempGyro += Gyro_ADC;
		if(cnt_g == CALIBRATING_GYRO_CYCLES)
		{
			Gyro_Offset.x = tempGyro.x/cnt_g;
			Gyro_Offset.y = tempGyro.y/cnt_g;
			Gyro_Offset.z = tempGyro.z/cnt_g;
			cnt_g = 0;
			Gyro_CALIBRATED = 0;
			//param.SAVE_GYRO_OFFSET();//保存数据
			return;
		}
		cnt_g++;
	}
}


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
