#include "PandaClient.h"
#include "ui_PandaClient.h"
#include <QDebug>
#include <QFontComboBox>
#include <QMessageBox>
PandaClient::PandaClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PandaClient)
{
    ui->setupUi(this);
    TcpClient = new QTcpSocket(this);   //实例化tcpClient
    TcpClient->abort();                 //取消原有连接
    connect(TcpClient, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
    connect(ui->PushButton_Send,&QPushButton::clicked,this,&PandaClient::SlotSend);

    TcpClient->connectToHost("frp-car.top", 37330);
    TcpClient->waitForConnected(3000);
    if (TcpClient->state() == QAbstractSocket::UnconnectedState)
    {
        QMessageBox::information(this,"提示","连接失败，请重新连接");

    }


    LoginIn = new Login(TcpClient);
//    this->show();
    LoginIn->show();
    connect(LoginIn, &Login::SignShowClient, this, &QMainWindow::show);
    ColorInit();

}

PandaClient::~PandaClient()
{
    delete ui;
}

void PandaClient::ColorInit()
{
    this->setStyleSheet("background-color: rgb(20,196,188);");
    ui->frame_2->setStyleSheet("background-color: rgb(105,105,105);");
    ui->listWidget->setStyleSheet("background-color: rgb(169,169,169);");
    ui->frame_3->setStyleSheet("background-color: rgb(220,220,220);");
}

void PandaClient::FontInit()
{
    connect(ui->FontMode, &QFontComboBox::currentFontChanged, ui->TextEdit_Send, [=](const QFont &font){
            ui->TextEdit_Send->setFontFamily(font.toString());
            ui->TextEdit_Send->setFocus();
        });



}

void PandaClient::SlotReadData()
{
//    QByteArray buffer = TcpClient->readAll();

//    if(!buffer.isEmpty())
//    {
//        ui->TextEdit->append(buffer);
//    }
    {
        QByteArray buffer = TcpClient->readAll();
        if(buffer[0] == '0' || buffer[0] == '1' || buffer[0] == '2')
        {
            LoginIn->signInReceive(buffer[0]);
        }
        else if(buffer[0] == 's')
        {
            auto SendMsg = buffer.mid(2);
            ui->TextEdit->append(SendMsg);
        }
        else if(buffer[0] == 'v')
        {
            auto ServerMsg = buffer.mid(2);
            ui->TextEdit->append(ServerMsg);
        }
    }
}

void PandaClient::SlotSend()
{
    qDebug()<<"fasong";
    QString data = ui->TextEdit_Send->toPlainText();
        if(data != "")
        {
            data = "s " + data;
            TcpClient->write(data.toUtf8()); //qt5去除了.toAscii()
        }
}

void PandaClient::SlotFontMode(QFont &font)
{
    ui->TextEdit_Send->setFontFamily(font.toString());
    ui->TextEdit_Send->setFocus();
}


