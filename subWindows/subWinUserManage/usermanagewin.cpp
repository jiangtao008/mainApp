#include "usermanagewin.h"
#include "ui_usermanagewin.h"
#include <QDebug>
UserManageWin::UserManageWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManageWin)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->showFullScreen();
    myMenu = new QMenu();

    QAction* operation_query = myMenu->addAction("查询");
    connect(operation_query,&QAction::triggered,this,[=]{
        qDebug()<<"operation_query...";
    });

    QAction* operation_add = myMenu->addAction("新增");
    connect(operation_add,&QAction::triggered,this,[=]{
        qDebug()<<"operation_add...";
    });

    QAction* operation_edit = myMenu->addAction("修改");
    connect(operation_edit,&QAction::triggered,this,[=]{
        qDebug()<<"operation_edit...";
    });

    QAction* operation_delete = myMenu->addAction("删除");
    connect(operation_delete,&QAction::triggered,this,[=]{
        qDebug()<<"operation_delete...";
    });

    ui->pushButton_menu->setMenu(myMenu);

    QStringList colName;
    colName << "权限"<<"姓名"<<"职位"<<"工号";
    ui->tableWidget->setColumnCount(colName.size());
    ui->tableWidget->setHorizontalHeaderLabels(colName);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

UserManageWin::~UserManageWin()
{
    delete ui;
}

void UserManageWin::on_pushButtonClose_clicked()
{
    emit close();
    this->close();
}
