#ifndef USERMANAGEWIN_H
#define USERMANAGEWIN_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class UserManageWin;
}

class UserManageWin : public QWidget
{
    Q_OBJECT

public:
    explicit UserManageWin(QWidget *parent = nullptr);
    ~UserManageWin();

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::UserManageWin *ui;
    QMenu * myMenu;
};

#endif // USERMANAGEWIN_H
