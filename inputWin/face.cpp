#include "face.h"

Face::Face(QWidget *parent) : QWidget(parent)
{
    backImg = new QLabel(this);
    backImg->move(0,0);
    backImg->resize(400,400);
    backImg->setPixmap(QPixmap(":/picture/face.jpg"));

//    QPixmap *pixmap = new QPixmap(":/images/welcome_tlauto.png");
//    pixmap->scaled(ui->label->size(), Qt::KeepAspectRatio);
//    ui->label->setPixmap(*pixmap);
}
