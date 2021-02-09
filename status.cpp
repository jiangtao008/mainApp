#include "status.h"
#include "ui_status.h"
#include <QDateTime>
#include <wiringPi.h>
#include <QFile>
#include "raspiDevice/raspiTemp.h"

Status::Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    //myDataFormat = STATUS_YEAR_MONTH_DAY_24;
    connect(RaspiTemp::instance(),SIGNAL(sendTemp(QString)),this,SLOT(updateTemp(QString)));
    RaspiTemp::instance()->start();
    connect(&myTime,SIGNAL(timeout()),this,SLOT(updateDate()));
    myTime.start(1000);
}

Status::~Status()
{
    delete ui;
}

void Status::updateDate()
{
    switch(myDataFormat)
    {
    case STATUS_YEAR_MONTH_DAY_24:
        ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));      break;
    case STATUS_YEAR_MONTH_DAY_12:
        ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd h:mm:ss AP"));    break;
    case STATUS_MONTH_DAY_YEAR_24:
        ui->label_time->setText(QDateTime::currentDateTime().toString("MM-dd-yyyy hh:mm:ss"));      break;
    case STATUS_MONTH_DAY_YEAR_12:
        ui->label_time->setText(QDateTime::currentDateTime().toString("MM-dd-yyyy h:mm:ss AP"));    break;
    case STATUS_YEAR__MONTH__DAY_24:
        ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));      break;
    case STATUS_YEAR__MONTH__DAY_12:
        ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd h:mm:ss AP"));    break;
    case STATUS_MONTH__DAY__YEAR_24:
        ui->label_time->setText(QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss"));      break;
    case STATUS_MONTH__DAY__YEAR_12:
        ui->label_time->setText(QDateTime::currentDateTime().toString("MM/dd/yyyy h:mm:ss AP"));    break;
    default:
        ui->label_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));      break;
    }
}

void Status::updateTemp(QString temp)
{
    ui->label_temp->setText(temp + " °C");
}







