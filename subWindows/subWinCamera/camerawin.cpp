#include "camerawin.h"
#include "ui_camerawin.h"
#include <QImage>
#include <QDebug>

CameraWin::CameraWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWin)
    ,showImgTime(nullptr)
    ,cap(nullptr)
    ,showType(RAW_IMG)
    ,buttonGroup(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    cap = new VideoCapture();
    cap->open(0);		//打开摄像头
    if(!cap->isOpened())		//打开摄像头是否成功
    {
        ui->labelPicture->setText("camera open failed!");
    }
    else
    {
        showImgTime = new QTimer();
        connect(showImgTime,&QTimer::timeout,this,&CameraWin::on_showImg);
        showImgTime->start(SHOW_TIME);
    }
    buttonGroup = new QButtonGroup();
    buttonGroup->addButton(ui->pushButton_raw);
    buttonGroup->addButton(ui->pushButton_gray);
    buttonGroup->addButton(ui->pushButton_edge);
    buttonGroup->addButton(ui->pushButton_bin);
}
CameraWin::~CameraWin()
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
    if(buttonGroup)
    {
        delete buttonGroup;
    }
    delete ui;
}
void CameraWin::on_pushButton_Close_clicked()
{
    this->close();
}

void CameraWin::on_pushButton_OK_clicked()       //照相
{
    //showImgTime->stop();

    if(!cap->isOpened()) return;
    cv::Mat frame;// 从摄像头中抓取并返回每一帧
    cap->read(frame);

    qDebug()<<"size x:"<<frame.cols<<"  y:"<<frame.rows<<"   subThread:"<<QThread::currentThreadId();
    cv::resize(frame, frame, Size(200, 200));
    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage image((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);

    QLabel *saveImg = new QLabel(this);
    saveImg->move(0,0);
    saveImg->resize(200,200);
    saveImg->setPixmap(QPixmap::fromImage(image));
    saveImg->show();
    QTimer::singleShot(1000,this,[=]{
        delete saveImg;
    });
    //showImgTime->start(SHOW_TIME);
}

void CameraWin::on_pushButton_raw_clicked()
{
    showType = RAW_IMG;
}
void CameraWin::on_pushButton_gray_clicked()
{
    showType = GRAY_IMG;
}
void CameraWin::on_pushButton_edge_clicked()
{
    showType = EDGE_IMG;
}
void CameraWin::on_pushButton_bin_clicked()
{
    showType = BIN_IMG;
}

void CameraWin::on_showImg()
{
    Mat frame;
    cap->read(frame);

    if(!frame.empty())
    {
        if(showType == RAW_IMG)
        {
            QImage image = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
            ui->labelPicture->setPixmap(QPixmap::fromImage(image));
        }
        else if(showType == GRAY_IMG)
        {
            Mat imgBuff = toGray(frame);
            QImage image((const uchar*)imgBuff.data, imgBuff.cols, imgBuff.rows, QImage::Format_Indexed8);
            ui->labelPicture->setPixmap(QPixmap::fromImage(image));
        }
        else if(showType == EDGE_IMG)
        {
            Mat imgBuff = toEdge(frame,30,120);
            QImage image((const uchar*)imgBuff.data, imgBuff.cols, imgBuff.rows, QImage::Format_Grayscale8);
            ui->labelPicture->setPixmap(QPixmap::fromImage(image));
        }
        else if(showType == BIN_IMG)
        {
            Mat imgBuff = toBinaryzation(frame,100);
            QImage image((const uchar*)imgBuff.data, imgBuff.cols, imgBuff.rows, QImage::Format_Indexed8);
            ui->labelPicture->setPixmap(QPixmap::fromImage(image));
        }
    }
}

Mat CameraWin::toGray(Mat src)
{
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY); // 【2】将原图像转换为灰度图像
    return gray;
}

Mat CameraWin::toEdge(Mat src,unsigned char thresh1,unsigned char thresh2)
{
    qDebug()<<"subThread:"<<QThread::currentThreadId();
    Mat gray,filter,dst;
    cvtColor(src,gray,CV_BGR2GRAY);
    blur(gray,filter,Size(3,3));        //滤波降噪
    Canny(filter,dst,thresh1,thresh2);  //30,200
    return dst;
}

Mat CameraWin::toBinaryzation(Mat src,unsigned char thresh)
{
    Mat dst,edge,gray;
    //dst.create(src.size(),src.type());    //创建与src同类型和大小的矩阵(dst)
    cvtColor(src, gray, CV_BGR2GRAY ); //将原图像转换为灰度图像
    blur( gray, edge, Size(3,3) );  //先用使用 3x3内核来降噪
    threshold(edge, dst, thresh, 255, THRESH_BINARY);   //type选THRESH_BINARY，大于阈值的设置为maxval(255),其它置0
    return dst;
}



