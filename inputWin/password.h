#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
class Password :public QWidget
{
    Q_OBJECT
public:
    explicit Password(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *backImg;
};

#endif // PASSWORD_H
