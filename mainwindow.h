#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "serial.h"

#include "usermanagewin.h"
#include "settingwin.h"
#include "aboutappwin.h"
#include "camerawin.h"
#include "carwin.h"
#include "monitorwin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_turn_clicked();
    void on_pushButton_about_clicked();
    void on_pushButton_setting_clicked();
    void on_pushButton_camera_clicked();
    void on_pushButton_quit_clicked();
    void on_pushButton_users_clicked();
    void on_pushButton_quit_pressed();
    void on_pushButton_quit_released();
    void on_pushButton_car_clicked();
    void on_pushButton_monitor_clicked();

private:
    Ui::MainWindow *ui;
    void screenOff();
    void screenOn();
    bool screenState;
    QTimer closeTime;
    QTimer *buzzerTime;
    QTimer *angleTime;
    Serial *serial;

    //sub windows ptr
    UserManageWin   *subWinUserManage;
    SettingWin      *subWinSetting;
    AboutAppWin     *subWinAboutApp;
    CameraWin       *subWinCamera;
    CarWin          *subWinCar;
    MonitorWin      *subWinMonitor;
};

#endif // MAINWINDOW_H
