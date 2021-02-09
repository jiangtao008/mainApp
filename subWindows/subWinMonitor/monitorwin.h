#ifndef MONITORWIN_H
#define MONITORWIN_H

#include <QWidget>

namespace Ui {
class monitorWin;
}

class MonitorWin : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWin(QWidget *parent = nullptr);
    ~MonitorWin();

private:
    Ui::monitorWin *ui;
    char *buff;
};

#endif // MONITORWIN_H
