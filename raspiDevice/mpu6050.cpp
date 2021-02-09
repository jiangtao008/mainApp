#include "mpu6050.h"
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <qmath.h>

int fd;
bool initMPU6050()  // 初始化 mpu6050
{
    wiringPiSetup () ;
    fd = wiringPiI2CSetup (SlaveAddress);
    if (fd >= 0) // fd 为负数，说明IIC连接失败
    {
        wiringPiI2CWriteReg8(fd,PWR_MGMT_1,0x00); // 开启温度检测 关闭休眠
        wiringPiI2CWriteReg8(fd,SMPLRT_DIV, 0x07);
        wiringPiI2CWriteReg8(fd,CONFIG, 0x06);
        wiringPiI2CWriteReg8(fd,GYRO_CONFIG, 0x18);
        wiringPiI2CWriteReg8(fd,ACCEL_CONFIG, 0x01);
        return 1;
    }
    return 0;
}
short getData(int reg_address)    // 读取制定寄存器中的数据
{
    unsigned int regH = wiringPiI2CReadReg8(fd,reg_address);
    unsigned int regL = wiringPiI2CReadReg8(fd,reg_address + 1);
    short reg = (regH << 8) | regL;
    return reg;
}

short readAccX(){  return getData(ACCEL_XOUT_H); }
short readAccY(){  return getData(ACCEL_YOUT_H); }
short readAccZ(){  return getData(ACCEL_ZOUT_H); }
short readGyroX(){ return getData(GYRO_XOUT_H);  }
short readGyroY(){ return getData(GYRO_YOUT_H);  }
short readGyroZ(){ return getData(GYRO_ZOUT_H);  }
short readTemp(){  return getData(TEMP_OUT_H);   }


#define Kp 100.0f           // 比例增益支配率收敛到加速度计/磁强计
#define Ki 0.002f           // 积分增益支配率的陀螺仪偏见的衔接
#define halfT 0.005f        // 采样周期的一半

float Yaw,Pitch,Roll;                       //偏航角，俯仰角，翻滚角

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float q0 = 1, q1 = 0, q2 = 0, q3 = 0;       // 四元数的元素，代表估计方向
    float exInt = 0, eyInt = 0, ezInt = 0;      // 按比例缩小积分误差
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // 测量正常化
    norm = sqrt(ax*ax + ay*ay + az*az);
    ax = ax / norm;                   //单位化
    ay = ay / norm;
    az = az / norm;

    // 估计方向的重力
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    // 积分误差比例积分增益
    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki;
    ezInt = ezInt + ez*Ki;

    // 调整后的陀螺仪测量
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    // 整合四元数率和正常化
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // 正常化四元
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,转换为度数
    Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv
    //Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;                //此处没有价值，注掉
}

void getAngle_IMU(float *angle)
{
    float ax = readAccX();
    float ay = readAccY();
    float az = readAccZ();
    float gx = readGyroX();
    float gy = readGyroY();
    float gz = readGyroZ();
    IMUupdate(gx, gy, gz, ax, ay, az);
    angle[0] = Pitch;
    angle[1] = Roll;
    angle[2] = Yaw;
}

//卡尔曼滤波参数与函数
float dt = 0.01;                   //注意：dt的取值为kalman滤波器采样时间
float Q_angle=0.001, Q_gyro=0.005;  //角度数据置信度,角速度数据置信度
float Kalman_Filter(float angle_m, float gyro_m)//angleAx 和 gyroGy
{
    float angle = 0, angle_dot;//角度和角速度
    float R_angle=0.5 ,C_0 = 1;
    float q_bias = 0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

    angle+=(gyro_m-q_bias) * dt;
    angle_err = angle_m - angle;

    float P[2][2] = {{ 1, 0 },
                     { 0, 1 }};
    float Pdot[4] ={ 0,0,0,0};
    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;

    angle += K_0 * angle_err; //最优角度
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m-q_bias;//最优角速度

    return angle;
}
void getAngle_Kalman(float *angle)
{
    float ax = readAccX();
    float ay = readAccY();
    float az = readAccZ();
    float gx = readGyroX()/7510.0; //陀螺仪得到的角速度
    float gy = readGyroY()/7510.0;
    float gz = readGyroZ()/7510.0;

    float angleAx = atan(ax/az)*57.3;     //加速度得到的角度
    float angleAy = atan(ay/az)*57.3;     //加速度得到的角度
    float angleAz = atan(ax/ay)*57.3;     //加速度得到的角度

    angle[0] = Kalman_Filter(angleAx,gy);
}

//一阶互补滤波

float K1 =0.1; // 对加速度计取值的权重
float dt_yijie=0.01;//注意：dt的取值为滤波器采样时间

float yijiehubu(float angle_m, float gyro_m , float currentAngle)//采集后计算的角度和角加速度
{
    currentAngle = K1 * angle_m + (1-K1) * (currentAngle + gyro_m * dt_yijie);
    return currentAngle;
}

void getAngle_yijie(float *angle)
{
    static float currentAngle_x = 0;
    static float currentAngle_y = 0;
    static float currentAngle_z = 0;
    float ax = readAccX() - 1000;
    float ay = readAccY() - 120;
    float az = readAccZ();
    float gx = readGyroX()/7510.0; //陀螺仪得到的角速度
    float gy = readGyroY()/7510.0;
    float gz = readGyroZ()/7510.0;


    float angleAx = atan(ax/az) * 57.3;
    float angleAy = atan(ay/az) * 57.3;
    float angleAz = atan(ax/ay) * 57.3;

    currentAngle_x = yijiehubu(angleAx,gy,currentAngle_x);
    currentAngle_y = yijiehubu(angleAx,gy,currentAngle_y);
    currentAngle_z = yijiehubu(angleAx,gy,currentAngle_z);

#if 0
    angle[0] = ax;
    angle[1] = ay;
    angle[2] = az;
#endif

#if 0
    angle[0] = gx;
    angle[1] = gy;
    angle[2] = gz;
#endif

#if 0
    angle[0] = angleAx;
    angle[1] = angleAy;
    angle[2] = angleAz;
#endif

#if 1
    angle[0] = currentAngle_x;
    angle[1] = currentAngle_y;
    angle[2] = currentAngle_z;
#endif

}



