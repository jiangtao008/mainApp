#ifndef SETTINGWIN_H
#define SETTINGWIN_H

#include <QWidget>

namespace Ui {
class SettingWin;
}

class SettingWin : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWin(QWidget *parent = nullptr);
    ~SettingWin();

private:
    Ui::SettingWin *ui;
};

#endif // SETTINGWIN_H
