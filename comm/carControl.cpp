#include "carControl.h"

CarControl::CarControl(QObject *parent) : QObject(parent)
{
    init();
}

CarControl::~CarControl()
{
    softPwmStop(Motor1PwmA);
    softPwmStop(Motor1PwmB);
    softPwmStop(Motor2PwmA);
    softPwmStop(Motor2PwmB);
}

void CarControl::init()
{
    qDebug()<<Q_FUNC_INFO<<"init gpio device...";
    wiringPiSetup();
    pinMode(ServoPwmA, PWM_OUTPUT);
    pwmSetMode (PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(2000);
    pwmWrite(ServoPwmA,0);
    pinMode(ServoPwmB, PWM_OUTPUT);
    pwmSetMode (PWM_MODE_MS);
    pwmSetClock(192);
    pwmSetRange(2000);
    pwmWrite(ServoPwmB,0);

    //    softPwmCreate(Motor1PwmA, 0,1000);
    //    softPwmCreate(Motor1PwmB,0,1000);
    //    softPwmCreate(Motor2PwmA, 0,1000);
    //    softPwmCreate(Motor2PwmB,0,1000);
    //    softPwmWrite(Motor1PwmA, 0);
    //    softPwmWrite(Motor1PwmB, 0);
    //    softPwmWrite(Motor2PwmA, 0);
    //    softPwmWrite(Motor2PwmB, 0);
}

#define ValueMax 2000       //输入范围最大值
#define RangeMin 50
#define RangeMax 250
void CarControl::carServo(int x,int y)      //range -1000 ~ +1000
{
    x += ValueMax/2;
    y += ValueMax/2;
    int range = RangeMax - RangeMin;
    x = x * range / ValueMax + RangeMin;
    y = y * range / ValueMax + RangeMin;
    qDebug()<<Q_FUNC_INFO<<QString("control servo X:%1  Y:%2").arg(x).arg(y);
    pwmWrite(ServoPwmA,x);
    pwmWrite(ServoPwmB,y);
}


void CarControl::carWS(int ws)
{
    int pwmA,pwmB;
    if(ws >= 0)
    {
        pwmA = ws;
        pwmB = 0;
    }
    else
    {
        pwmA = 0;
        pwmB = abs(ws);
    }
    softPwmWrite(Motor1PwmA, pwmA);
    softPwmWrite(Motor1PwmB, pwmB);
}
void CarControl::carDA(int da)
{
    int pwmA,pwmB;
    if(da >= 0)
    {
        pwmA = da;
        pwmB = 0;
    }
    else
    {
        pwmA = 0;
        pwmB = abs(da);
    }
    softPwmWrite(Motor2PwmA, pwmA);
    softPwmWrite(Motor2PwmB, pwmB);
}

void CarControl::tcpReceive(int i, QByteArray data)
{
    uchar headerH = data.at(0);
    uchar headerL = data.at(1);
    if(headerH != 0xfa || headerL != 0xfb)
        return;
    int offset = 3;
    Int2Byte dataBuff;
    dataBuff.byteData[0] = data.at(offset++);   //3
    dataBuff.byteData[1] = data.at(offset++);   //4
    dataBuff.byteData[2] = data.at(offset++);   //5
    dataBuff.byteData[3] = data.at(offset++);   //6
    int controlWS = dataBuff.intData;

    dataBuff.byteData[0] = data.at(offset++);
    dataBuff.byteData[1] = data.at(offset++);
    dataBuff.byteData[2] = data.at(offset++);
    dataBuff.byteData[3] = data.at(offset++);
    int controlDA = dataBuff.intData;

    dataBuff.byteData[0] = data.at(offset++);
    dataBuff.byteData[1] = data.at(offset++);
    dataBuff.byteData[2] = data.at(offset++);
    dataBuff.byteData[3] = data.at(offset++);
    int controlServoX = dataBuff.intData;

    dataBuff.byteData[0] = data.at(offset++);
    dataBuff.byteData[1] = data.at(offset++);
    dataBuff.byteData[2] = data.at(offset++);
    dataBuff.byteData[3] = data.at(offset++);
    int controlServoY = dataBuff.intData;

    qDebug()<<"controlWS:"<<controlWS
            <<"   controlDA:"<<controlDA
            <<"   controlServoX:" <<controlServoX
            <<"   controlServoY:"<<controlServoY;
    carServo(controlServoX,controlServoY);
}
