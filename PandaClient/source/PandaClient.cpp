#include "PandaClient.h"
#include "ui_PandaClient.h"
#include <QDebug>
#include <QMessageBox>
PandaClient::PandaClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PandaClient)
{
    ui->setupUi(this);
    TcpClient = new QTcpSocket(this);   //实例化tcpClient
    TcpClient->abort();                 //取消原有连接
    connect(TcpClient, SIGNAL(readyRead()), this, SLOT(SlotReadData()));


    TcpClient->connectToHost("frp-bus.top", 37245);
    TcpClient->waitForConnected(3000);
    if (TcpClient->state() == QAbstractSocket::UnconnectedState)
    {
        QMessageBox::information(this,"提示","连接失败，请重新连接");

    }

}

PandaClient::~PandaClient()
{
    delete ui;
}

void PandaClient::SlotReadData()
{
    QByteArray buffer = TcpClient->readAll();
    if(!buffer.isEmpty())
    {
        ui->TextEdit->append(buffer);
     }

}





void PandaClient::SlotSend()
{
    qDebug()<<"fasong";
    QString data = ui->TextEdit_Send->toPlainText();
        if(data != "")
        {
            TcpClient->write(data.toLatin1()); //qt5去除了.toAscii()
        }
}
