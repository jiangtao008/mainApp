#include "settingwin.h"
#include "ui_settingwin.h"

SettingWin::SettingWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

SettingWin::~SettingWin()
{
    delete ui;
}
