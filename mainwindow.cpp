#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  <QImage>
#include <wiringPi.h>
#include "mpu6050.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,subWinUserManage(nullptr)
    ,subWinSetting(nullptr)
    ,subWinAboutApp(nullptr)
    ,subWinCamera(nullptr)
    ,subWinCar(nullptr)
    ,subWinMonitor(nullptr)
{
    ui->setupUi(this);
    //QApplication::setOverrideCursor(Qt::BlankCursor);
    this->showFullScreen();

    QVector<QWidget*> subWinPtrVector;

    subWinPtrVector.append(subWinUserManage);
    subWinPtrVector.append(subWinSetting);
    subWinPtrVector.append(subWinAboutApp);
    subWinPtrVector.append(subWinCamera);
    subWinPtrVector.append(subWinCar);
    subWinPtrVector.append(subWinMonitor);

    wiringPiSetup();        //初始化gpio
    initMPU6050();

    serial = new Serial();
    if(serial->open())
        qDebug()<<"open serial sucessed!";
    else
        qDebug()<<"open serial failed!";
    connect(serial,&Serial::readyRead,this,[=]()
    {
        qDebug()<<serial->read();
    });

    angleTime = new QTimer();
    connect(angleTime,&QTimer::timeout,this,[=]()
    {
        float angle[3];
        getAngle_yijie(angle);
        serial->OutPut(angle[0],angle[1],angle[2],5.0);
    });
    angleTime->start(10);
    qDebug()<<"   main Thread:"<<QThread::currentThreadId();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_about_clicked()      //关于本机
{
    subWinAboutApp = new AboutAppWin();
    subWinAboutApp->show();
}
void MainWindow::on_pushButton_setting_clicked()    //设置界面
{
    subWinSetting = new SettingWin();
    subWinSetting->show();
}
void MainWindow::on_pushButton_camera_clicked()     //相机
{
    subWinCamera = new CameraWin();
    subWinCamera->show();
}
void MainWindow::on_pushButton_users_clicked()      //用户列表（管理员模式下）
{
    subWinUserManage = new UserManageWin();
    subWinUserManage->show();
}
void MainWindow::on_pushButton_car_clicked()
{
    subWinCar = new CarWin();
    subWinCar->show();
}
void MainWindow::on_pushButton_monitor_clicked()
{
    subWinMonitor = new MonitorWin();
    subWinMonitor->show();
}

void MainWindow::on_pushButton_turn_clicked()       //输入切换
{
    ui->widget_input->turnRight(500);
}

void MainWindow::on_pushButton_quit_clicked()       //退出软件
{
    if(!screenState)
        return ;
    screenOff();
}

void MainWindow::on_pushButton_quit_pressed()
{

}
void MainWindow::on_pushButton_quit_released()
{

}

void MainWindow::screenOn()     //屏幕（休眠）唤醒
{
    int res = system("xset dpms force on");
    if(res)
        screenState = 1;
}
void MainWindow::screenOff()    //进入休眠
{
    int res = system("xset dpms force off");
    if(res)
        screenState = 0;
}
void init()
{

}

