#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr);

    bool open(QString name = "/dev/ttyS0", unsigned int speed = 115200, unsigned char dataBits = 8, unsigned char stopBits = 1);
    void close();
    bool send(QByteArray data);
    bool send(QString data);
    bool send(char *data, qint64 len);
    QByteArray read();
    void OutPut(float s1, float s2, float s3, float s4);
signals:
    void readyRead();
public slots:

private:
    QSerialPort *myCom;
    QString comName;
    unsigned int  comBaudRate;
    unsigned char comDataBits;
    unsigned char comStopBits;
    unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
};

#endif // SERIAL_H
