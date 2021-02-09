#include "fpc1020a.h"

FPC1020A::FPC1020A(QWidget *parent) :
    QWidget(parent)
    ,mSerial(nullptr)
{
    backImg = new QLabel(this);
    backImg->move(0,0);
    backImg->resize(400,400);
    backImg->setPixmap(QPixmap(":/picture/finger.jpg"));
}
