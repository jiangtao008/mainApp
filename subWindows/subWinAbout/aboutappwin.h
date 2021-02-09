#ifndef ABOUTAPPWIN_H
#define ABOUTAPPWIN_H

#include <QWidget>

namespace Ui {
class AboutAppWin;
}

class AboutAppWin : public QWidget
{
    Q_OBJECT

public:
    explicit AboutAppWin(QWidget *parent = nullptr);
    ~AboutAppWin();

private:
    Ui::AboutAppWin *ui;
};

#endif // ABOUTAPPWIN_H
