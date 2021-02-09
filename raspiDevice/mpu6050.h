#ifndef MPU6050_H
#define MPU6050_H

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0x68	//MPU6050器件地址

bool initMPU6050();             // 初始化 mpu6050
short getData(int reg_address);    // 读取制定寄存器中的数据

short readAccX();
short readAccY();
short readAccZ();
short readGyroX();
short readGyroY();
short readGyroZ();
short readTemp();

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void getAngle_IMU(float *angle);

float Kalman_Filter(float angle_m, float gyro_m);//angleAx 和 gyroGy
void getAngle_Kalman(float *angle);

float yijiehubu(float angle_m, float gyro_m);
void getAngle_yijie(float *angle);

#endif // MPU6050_H
