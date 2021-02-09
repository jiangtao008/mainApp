#ifndef RASPITEMP_H
#define RASPITEMP_H

#include <QThread>
#include <QObject>
#include <QString>
#include <QFile>

#define TEMP_FILE "/sys/bus/w1/devices/28-011830ef0dff/w1_slave"

class RaspiTemp : public QThread
{
    Q_OBJECT
public:
    static RaspiTemp* instance(){ return instancePrt; }
    QString getTemp()
    {
        return tempStr;
    }

signals:
    void sendTemp(QString temp);

public slots:

protected:
    void run();

private:
    static RaspiTemp *instancePrt;
    QFile *tempFile;
    QString tempStr;
    explicit RaspiTemp();
    ~RaspiTemp(){}
    QString readTemp();
};

#endif // RASPITEMP_H
