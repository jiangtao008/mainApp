#ifndef TcpServer_H
#define TcpServer_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QMutex>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

static QMutex myMutex;
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void close();
    bool listen(quint16 port,QHostAddress ip = QHostAddress::Any);

    QList<QTcpSocket*> getAllClientSocket()
    {
        return tcpClient;
    }

    void sendData(QByteArray data);
    void sendData(int i, QByteArray data);

signals:
    //服务器有新增连接和断开连接，都会发送此信号
    void s_connectChange();
    void s_TcpHaveRead(int i,QByteArray);

private slots:
    void onTcpNewConnect();
    void onTcpDisconnect();
    void onTcpHaveRead();
    void onSendImg(const Mat img);

private:
    QTcpServer *myServer;
    QList<QTcpSocket*> tcpClient;
    QTcpSocket *currentClient;
    QUdpSocket *myUdp;
    void udpSend(QByteArray data);
};

#endif // TcpServer_H
