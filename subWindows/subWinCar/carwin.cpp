#include "carwin.h"
#include "ui_carwin.h"

CarWin::CarWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CarWin)
    ,cap(nullptr)
    ,showImgTime(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    cap = new VideoCapture();
    cap->open(0);		//打开摄像头
    if(!cap->isOpened())		//打开摄像头是否成功
    {
        ui->label_show->setText("camera open failed!");
    }
    else
    {
        showImgTime = new QTimer(this);
        connect(showImgTime,&QTimer::timeout,this,&CarWin::on_showImg);
        showImgTime->start(30);
    }
}
CarWin::~CarWin()
{
    if(showImgTime)
    {
        if(showImgTime->isActive())
            showImgTime->stop();
        delete showImgTime;
    }
    if(cap)
    {
        if(cap->isOpened())
            cap->release();
        delete cap;
    }
    delete ui;
}
void CarWin::on_showImg()
{
    cap->read(frame);
    if(!frame.empty())
    {
        QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
        ui->label_show->setPixmap(QPixmap::fromImage(image));
    }
}

void CarWin::on_pushButtonClose_clicked()
{
    this->close();
}


