#ifndef CARWIN_H
#define CARWIN_H

#include <QWidget>
#include <QTimer>

#include <highgui.h>
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Ui {
class CarWin;
}

class CarWin : public QWidget
{
    Q_OBJECT

public:
    explicit CarWin(QWidget *parent = nullptr);
    ~CarWin();

private slots:
    void on_pushButtonClose_clicked();

    void on_showImg();

private:
    Ui::CarWin *ui;

    VideoCapture *cap;
    QTimer *showImgTime;

    Mat frame;
};

#endif // CARWIN_H
