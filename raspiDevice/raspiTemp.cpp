#include "raspiTemp.h"
#include <QDebug>

RaspiTemp *RaspiTemp::instancePrt = new RaspiTemp();

RaspiTemp::RaspiTemp()
{
    tempFile = new QFile();
    tempFile->setFileName(TEMP_FILE);
    tempFile->open(QIODevice::ReadOnly);
}

void RaspiTemp::run()
{
    while(1)
    {
        sleep(3);
        QString temp = readTemp();
        tempStr = temp;
        emit sendTemp(temp);
        //qDebug()<<Q_FUNC_INFO<<"Read temp:"<<temp;
    }
}

QString RaspiTemp::readTemp()
{
    if(!tempFile->isOpen())
        tempFile->open(QIODevice::ReadOnly);
    if(tempFile->isOpen())
    {
        QString tempStr(tempFile->readAll());
        tempFile->close();
        tempStr = tempStr.mid(tempStr.indexOf("t=")+2).trimmed();
        if(tempStr.size() < 4)
            return "--";
        tempStr.insert(tempStr.size()-3,".");
        return tempStr;
    }
    return "--";
}
