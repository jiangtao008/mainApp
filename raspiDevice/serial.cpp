#include "serial.h"

Serial::Serial(QObject *parent) : QObject(parent)
{
    myCom = new QSerialPort();
    connect(myCom,SIGNAL(readyRead()),this,SIGNAL(readyRead()));
}

bool Serial::open(QString name,unsigned int speed,unsigned char dataBits,unsigned char stopBits)
{
    myCom->setPortName(name);
    switch (speed)
    {
        case 1200:      myCom->setBaudRate(QSerialPort::Baud1200);      break;
        case 2400:      myCom->setBaudRate(QSerialPort::Baud2400);      break;
        case 4800:      myCom->setBaudRate(QSerialPort::Baud4800);      break;
        case 9600:      myCom->setBaudRate(QSerialPort::Baud9600);      break;
        case 19200:     myCom->setBaudRate(QSerialPort::Baud19200);     break;
        case 38400:     myCom->setBaudRate(QSerialPort::Baud38400);     break;
        case 57600:     myCom->setBaudRate(QSerialPort::Baud57600);     break;
        case 115200:    myCom->setBaudRate(QSerialPort::Baud115200);    break;
        default:        myCom->setBaudRate(QSerialPort::Baud115200);    break;
    }
    switch (dataBits)
    {
        case 5:      myCom->setDataBits(QSerialPort::Data5);      break;
        case 6:      myCom->setDataBits(QSerialPort::Data6);      break;
        case 7:      myCom->setDataBits(QSerialPort::Data7);      break;
        case 8:      myCom->setDataBits(QSerialPort::Data8);      break;
        default:     myCom->setBaudRate(QSerialPort::Data8);    break;
    }
    switch (stopBits)
    {
        case 1:      myCom->setStopBits(QSerialPort::OneStop);          break;  //1
        case 2:      myCom->setStopBits(QSerialPort::TwoStop);          break;  //2
        case 3:      myCom->setStopBits(QSerialPort::OneAndHalfStop);   break;  //1.5
        default:     myCom->setStopBits(QSerialPort::OneStop);          break;
    }
    myCom->setParity(QSerialPort::NoParity);
    myCom->setFlowControl(QSerialPort::NoFlowControl);
    return myCom->open(QIODevice::ReadWrite);
}

void Serial::close()
{
    myCom->close();
}

bool Serial::send(QByteArray data)
{
    quint64 dataSize = data.size();
    quint64 sendSize = 999999;
    if(!myCom->isOpen())
        return 0;
    sendSize = myCom->write(data);
    return dataSize == sendSize;
}
bool Serial::send(QString data)
{
    quint64 dataSize = data.size();
    quint64 sendSize = 999999;
    if(!myCom->isOpen())
        return 0;
    sendSize = myCom->write(data.toLatin1());
    return dataSize == sendSize;
}
bool Serial::send(char *data, qint64 len)
{
    quint64 sendSize = 999999;
    if(!myCom->isOpen())
        return 0;
    sendSize = myCom->write(data,len);
    return len == sendSize;
}

QByteArray Serial::read()
{
    return myCom->readAll();
}

//----------------------示波器---------------------------
void Serial::OutPut(float s1,float s2,float s3,float s4)
{
    float OutData[4] = {s1,s2,s3,s4};
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {
        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];
    }
    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }
    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;
    this->send((char *)databuf,10);
}
unsigned short Serial::CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;
    for (i=0;i<CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}


