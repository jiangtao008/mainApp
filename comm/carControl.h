#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <QObject>
#include <QDebug>

#include <wiringPi.h>
#include "softPwm.h"

#define Motor1PwmA 28
#define Motor1PwmB 29
#define Motor2PwmA 30
#define Motor2PwmB 31

#define ServoPwmA 1
#define ServoPwmB 24

#define Buzzer 3

union Int2Byte
{
    int intData;
    char byteData[4];
};

class CarControl : public QObject
{
    Q_OBJECT
public:
    explicit CarControl(QObject *parent = nullptr);
    ~CarControl();

    void carServo(int x, int y);
    void carDA(int da);
    void carWS(int ws);
    void init();
signals:

public slots:
    void tcpReceive(int i,QByteArray data);

};

#endif // CARCONTROL_H
