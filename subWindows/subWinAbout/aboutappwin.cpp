#include "aboutappwin.h"
#include "ui_aboutappwin.h"

AboutAppWin::AboutAppWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutAppWin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

AboutAppWin::~AboutAppWin()
{
    delete ui;
}
