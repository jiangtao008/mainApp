#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QTimer>
#include <QFile>

namespace Ui {
class Status;
}


enum DateFormat
{
    STATUS_YEAR_MONTH_DAY_24=0,
    STATUS_YEAR_MONTH_DAY_12,
    STATUS_MONTH_DAY_YEAR_24,
    STATUS_MONTH_DAY_YEAR_12,
    STATUS_YEAR__MONTH__DAY_24,
    STATUS_YEAR__MONTH__DAY_12,
    STATUS_MONTH__DAY__YEAR_24,
    STATUS_MONTH__DAY__YEAR_12
};

class Status : public QWidget
{
    Q_OBJECT

public:
    explicit Status(QWidget *parent = 0);
    ~Status();
    void setDataFormat(DateFormat format){  myDataFormat = format;  }

protected slots:
    void updateDate();
    void updateTemp(QString temp);

private:
    Ui::Status *ui;
    QTimer myTime;
    DateFormat myDataFormat;
};


#endif // STATUS_H
