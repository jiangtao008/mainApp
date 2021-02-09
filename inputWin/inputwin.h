#ifndef INPUTWIN_H
#define INPUTWIN_H

#include <QWidget>
#include <QLabel>


namespace Ui {
class InputWin;
}

class InputWin : public QWidget
{
    Q_OBJECT

public:
    explicit InputWin(QWidget *parent = nullptr);
    ~InputWin();
    void turnRight(int time);
    void turnLeft(int time);
protected:
    bool event(QEvent *event);

private slots:

private:
    Ui::InputWin *ui;
    QLabel labPicture,labPicture2;


};

#endif // INPUTWIN_H
