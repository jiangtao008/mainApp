#include "inputwin.h"
#include "ui_inputwin.h"
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>

InputWin::InputWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputWin)
{
    ui->setupUi(this);
    labPicture.setParent(this);
    labPicture.move(ui->stackedWidget->pos());
    labPicture.resize(ui->stackedWidget->width(),ui->stackedWidget->height());
    labPicture.hide();
    labPicture2.setParent(this);
    labPicture2.move(ui->stackedWidget->pos());
    labPicture2.resize(ui->stackedWidget->width(),ui->stackedWidget->height());
    labPicture2.hide();
    ui->stackedWidget->setCurrentIndex(1);
}

InputWin::~InputWin()
{
    delete ui;
}

bool InputWin::event(QEvent *event)
{
    static int press_x;  //鼠标按下时的位置
    static int press_y;
    static int relea_x;  //鼠标释放时的位置
    static int relea_y;

    QMouseEvent *mevent = static_cast<QMouseEvent *>(event); //将之转换为鼠标事件

    if(mevent->type()==QEvent::MouseButtonPress)  //如果鼠标按下
    {
        press_x = mevent->globalX();
        press_y = mevent->globalY();
    }

    if(mevent->type()==QEvent::MouseButtonRelease)  //如果鼠标释放
    {
         relea_x = mevent->globalX();
         relea_y = mevent->globalY();
    }

    //判断滑动方向（右滑 ->  切换到左边）
    if((relea_x - press_x)>20 && mevent->type()==QEvent::MouseButtonRelease)
    {
        int current_page = ui->stackedWidget->currentIndex()-1;
        if(current_page < 0)
        {
            current_page = 0;
            return QWidget::event(event);
        }
        turnLeft(750);
    }

    //判断滑动方向（左滑<- 切换到右边）
    if((press_x - relea_x)>20 && mevent->type()==QEvent::MouseButtonRelease)
    {
        int current_page = ui->stackedWidget->currentIndex()+1;
        if(current_page >= ui->stackedWidget->count())
        {
            current_page = ui->stackedWidget->count()-1;
            return QWidget::event(event);
        }
        turnRight(750);
    }
    return QWidget::event(event);
}

void InputWin::turnRight(int time)
{
    labPicture.show();
    labPicture.setPixmap(ui->stackedWidget->currentWidget()->grab());
    QPropertyAnimation *animation1 = new QPropertyAnimation(&labPicture,"geometry",this);
    animation1->setDuration(time);
    animation1->setStartValue(QRect(ui->stackedWidget->pos().x(),ui->stackedWidget->pos().y(),
                            ui->stackedWidget->width(),ui->stackedWidget->height()));
    animation1->setEndValue(QRect(-ui->stackedWidget->width(),ui->stackedWidget->pos().y(),
                            ui->stackedWidget->width(),ui->stackedWidget->height()));

    int nextPage = ui->stackedWidget->currentIndex() + 1;
    if(nextPage >= ui->stackedWidget->count())
        nextPage = 0;
    ui->stackedWidget->setCurrentIndex(nextPage);  //切换界面  //切换到下一个页面

    labPicture2.show();
    labPicture2.setPixmap(ui->stackedWidget->currentWidget()->grab());  //label显示的图片
    QPropertyAnimation *animation2 = new QPropertyAnimation(&labPicture2,"geometry",this);
    animation2->setDuration(time);       //设置动画时间
    //设置起始位置
    animation2->setStartValue(QRect(ui->stackedWidget->width(),ui->stackedWidget->pos().y(),
                                  ui->stackedWidget->width(),ui->stackedWidget->height()));
    //设置结束位置
    animation2->setEndValue(QRect(ui->stackedWidget->pos().x(),ui->stackedWidget->pos().y(),
                                  ui->stackedWidget->width(),ui->stackedWidget->height()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animation1);    //加入动画组类
    group->addAnimation(animation2);
    ui->stackedWidget->hide();  //启动动画前需要隐藏控件，不然位置处理不好会造成背景显示问题。
    group->start(); //启动动画
    connect(group,&QParallelAnimationGroup::finished,this,[=]()     //动画结束后信号槽
    {
        ui->stackedWidget->show();  //显示控件（启动之前被隐藏）
        labPicture.hide();      //隐藏移动的图片
        labPicture2.hide();
        delete animation1;      //释放函数内部申请的资源。
        delete animation2;
        delete group;
    });
}

void InputWin::turnLeft(int time)
{
    labPicture.setPixmap(ui->stackedWidget->currentWidget()->grab());  //捕获当前界面并绘制到label上
    labPicture.show();
    QPropertyAnimation *animation1 = new QPropertyAnimation(&labPicture,"geometry");
    animation1->setDuration(time);  //设置动画时间为1秒
    animation1->setStartValue(QRect(ui->stackedWidget->pos().x(),ui->stackedWidget->pos().y(),
                                    ui->stackedWidget->width(),ui->stackedWidget->height()));
    animation1->setEndValue(QRect(ui->stackedWidget->width(),ui->stackedWidget->pos().y(),
                                  ui->stackedWidget->width(),ui->stackedWidget->height()));
    int nextPage = ui->stackedWidget->currentIndex() - 1;
    if(nextPage < 0)
        nextPage = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(nextPage);  //切换界面
    labPicture2.show();
    labPicture2.setPixmap(ui->stackedWidget->currentWidget()->grab());
    QPropertyAnimation *animation2 = new QPropertyAnimation(&labPicture2,"geometry");
    animation2->setDuration(time);
    animation2->setStartValue(QRect(-ui->stackedWidget->width(),ui->stackedWidget->pos().y(),
                                    ui->stackedWidget->width(),ui->stackedWidget->height()));
    animation2->setEndValue(QRect(ui->stackedWidget->pos().x(),ui->stackedWidget->pos().y(),
                                  ui->stackedWidget->width(),ui->stackedWidget->height()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animation1);
    group->addAnimation(animation2);
    ui->stackedWidget->hide();
    group->start();
    connect(group,&QParallelAnimationGroup::finished,this,[=]()
    {
        ui->stackedWidget->show();
        labPicture.hide();
        labPicture2.hide();
        delete animation1;
        delete animation2;
        delete group;
    });
}


