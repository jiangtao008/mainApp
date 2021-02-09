#ifndef FPC1020A_H
#define FPC1020A_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include<QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class FPC1020A : public QWidget
{
    Q_OBJECT
public:
    explicit FPC1020A(QWidget *parent = nullptr);
    void setSerial(QSerialPort *serial)
    {
        mSerial = serial;
    }
signals:

public slots:


private:
    QSerialPort *mSerial;
    QLabel *backImg;
};

#endif // FPC1020A_H
