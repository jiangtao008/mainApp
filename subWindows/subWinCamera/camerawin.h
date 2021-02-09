#ifndef CAMERAWIN_H
#define CAMERAWIN_H

#include <QWidget>
#include <QImage>
#include <QTimer>

#include <highgui.h>
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QThread>
#include <QButtonGroup>

namespace Ui {
class CameraWin;
}
using namespace cv;

class CameraWin : public QWidget
{
    Q_OBJECT
#define SHOW_TIME 30
    enum ShowType
    {
        RAW_IMG = 0,
        GRAY_IMG,
        EDGE_IMG,
        BIN_IMG
    };
public:
    explicit CameraWin(QWidget *parent = nullptr);
    ~CameraWin();

private slots:
    void on_pushButton_OK_clicked();
    void on_pushButton_Close_clicked();
    void on_pushButton_raw_clicked();
    void on_pushButton_gray_clicked();
    void on_pushButton_edge_clicked();
    void on_pushButton_bin_clicked();

    void on_showImg();

private:
    Ui::CameraWin *ui;
    QTimer *showImgTime;
    VideoCapture *cap;
    ShowType showType;
    QButtonGroup *buttonGroup;
    void showImg();

    Mat toGray(Mat src);
    Mat toEdge(Mat src, unsigned char thresh1, unsigned char thresh2);
    Mat toBinaryzation(Mat src, unsigned char thresh);
};

#endif // CAMERAWIN_H
