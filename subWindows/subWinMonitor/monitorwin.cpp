#include "monitorwin.h"
#include "ui_monitorwin.h"

MonitorWin::MonitorWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monitorWin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    buff = new char[1024*1024*10];
}

MonitorWin::~MonitorWin()
{
    delete buff;
    delete ui;
}
