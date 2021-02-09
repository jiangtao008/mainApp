#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
/***************************************
接口如下:
新增、修改、删除、清空。
1、用户接口
    用户名、用户其他基本信息、用户权限
2、密码数据操作接口 密码保存方式：用户密码+内部私钥（随机内容文件） = MD5（程序中跑的密码）
    密码（md5加密后的数据）
3、指纹数据操作接口
    对应指纹模块中的ID号
4、脸部数据操作接口
    暂时未定
****************************************/
class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DATABASE_H


