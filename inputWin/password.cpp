#include "password.h"
#include <QPalette>
Password::Password(QWidget *parent) : QWidget(parent)
{
    this->move(0,0);
    backImg = new QLabel(this);
    backImg->move(0,0);
    backImg->resize(400,400);
    backImg->setPixmap(QPixmap(":/picture/password.jpg"));
}
