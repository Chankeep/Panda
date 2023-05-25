#include "PandaServer.h"
#include "ui_PandaServer.h"

#include <QDebug>

PandaServer::PandaServer(QWidget *parent)
    : QTcpServer(parent),
    ui(new Ui::PandaServer),
    DbManager(DatabaseManager::GetManager())
{
    mainWindow = new QMainWindow();
    ui->setupUi(this->mainWindow);
    connect(ui->ClientSendButton, &QPushButton::clicked, this, &PandaServer::SlotSendServerMsg);

    DbManager.Init();


    this->listen(QHostAddress::Any, 8080);

    qDebug() << "listen begin!";
}

PandaServer::~PandaServer()
{
    delete ui;
}

bool PandaServer::Check(const QString &account)
{
    if(DbManager.Find(account))
    {
        return true;
    }
    return false;
}

void PandaServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new connection. descriptor: " << socketDescriptor;

    PandaSocket* socket = new PandaSocket(socketDescriptor);

    connect(socket, &PandaSocket::SignSendMsg, this, &PandaServer::SlotSendClinetMsg);
    connect(socket, &PandaSocket::SignDisconnected, this, &PandaServer::SlotDisconnected);

    socket->setSocketDescriptor(socketDescriptor);
    PandaSocketList.append(socket);

}

void PandaServer::SlotSignUp()
{

}

void PandaServer::SlotSendServerMsg()
{
    QString msg = ui->serverSend->toPlainText();
    if(msg == "")
        return;
    qDebug() << "send message!";
    for(int i=0; i<PandaSocketList.count(); i++)
    {
        auto item = PandaSocketList.at(i);
        if(item->write(msg.toLatin1()))
        {
            qDebug() << "send to socket:" << item->socketDescriptor();
            continue;
        }
    }
}

void PandaServer::SlotSendClinetMsg(QString msg)
{
//    emit updateServer(msg, length);
    ui->socketSended->append(msg);
    for(int i=0; i<PandaSocketList.count(); i++)
    {
        auto item = PandaSocketList.at(i);
        if(item->write(msg.toLatin1()))
        {
            continue;
        }
    }
}

void PandaServer::SlotDisconnected(int descriptor)
{
    for(int i=0; i<PandaSocketList.count(); i++)
    {
        auto item = PandaSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            PandaSocketList.removeAt(i);
            return;
        }
    }
    return;
}
