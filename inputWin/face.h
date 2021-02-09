#ifndef FACE_H
#define FACE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
class Face : public QWidget
{
    Q_OBJECT
public:
    explicit Face(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *backImg;
};

#endif // FACE_H
