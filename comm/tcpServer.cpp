#include "tcpServer.h"


TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    myServer = new QTcpServer(this);
    connect(myServer, SIGNAL(newConnection()), this, SLOT(onTcpNewConnect()));
    myUdp = new QUdpSocket(this);
}

//======================================================================
//启动监听
bool TcpServer::listen(quint16 port, QHostAddress ip)
{
    bool ok = myServer->listen(ip, port);
    return ok;
}
//关闭服务器
void TcpServer::close()
{
    for(int i = tcpClient.size() -1 ;i > 0;i --)//断开所有连接
    {
        tcpClient[i]->disconnectFromHost();
        bool ok = tcpClient[i]->waitForDisconnected(1000);
        if(ok)
            tcpClient.removeAt(i);  //从保存的客户端列表中取去除
    }
    myServer->close();     //不再监听端口
}

//======================================================================
void TcpServer::onTcpNewConnect()
{
    currentClient = myServer->nextPendingConnection();
    tcpClient.append(currentClient);
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(onTcpHaveRead()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(onTcpDisconnect()));
    emit s_connectChange();
}
void TcpServer::onTcpDisconnect()
{
    for(int i=0; i<tcpClient.size(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            //删除存储在tcpClient列表中的客户端信息
            tcpClient[i]->destroyed();
            tcpClient.removeAt(i);
        }
    }
    emit s_connectChange();
}

//======================================================================
void TcpServer::onTcpHaveRead()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for(int i=0; i<tcpClient.size(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if(buffer.isEmpty())    continue;

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                    .arg(tcpClient[i]->peerPort());

        // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
        if(IP_Port != IP_Port_Pre)
            IP_Port_Pre = IP_Port;   //更新ip_port
        emit s_TcpHaveRead(i,buffer);
        qDebug()<<buffer;
    }
}

void TcpServer::onSendImg(const Mat img)
{
//    TcpServer::udpSend(QByteArray data)
//    {
//        myUdp->writeDatagram(data,QHostAddress("192.168.137.1"),6677);
//    }
}


void TcpServer::sendData(QByteArray data)
{
    for(int i=0; i<tcpClient.size(); i++)
        sendData(i,data);
}
void TcpServer::sendData(int i, QByteArray data)
{
    tcpClient[i]->write(data);
}


