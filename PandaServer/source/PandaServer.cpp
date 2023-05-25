#include "PandaServer.h"
#include "ui_PandaServer.h"

#include <QDebug>

PandaServer::PandaServer(QWidget *parent)
    : QTcpServer(parent),
    ui(new Ui::PandaServer),
    dbManager(DatabaseManager::GetManager())
{
    mainWindow = new QMainWindow();
    ui->setupUi(this->mainWindow);
    connect(ui->ClientSendButton, &QPushButton::clicked, this, &PandaServer::SlotSendServerMsg);

    dbManager.Init();
    InitThread();

    this->listen(QHostAddress::Any, 8080);

    qDebug() << "listen begin!";
}

PandaServer::~PandaServer()
{
    delete ui;
}

bool PandaServer::Check(const QString &account)
{
    if(dbManager.Find(account))
    {
        return true;
    }
    return false;
}

int PandaServer::GetMinLoadThreadIndex()
{
    int n = threadList.count();
    if(n == 0)
        return -1;
    quint16 minLoad = threadList[0]->CurrentSocketNum;
    quint8 index;
    for(int i=0; i<n; n++)
    {
        if(threadList[i]->CurrentSocketNum < minLoad)
        {
            index = i;
            minLoad = threadList[i]->CurrentSocketNum;
        }
    }
    return index;
}

void PandaServer::InitThread()
{
    auto threadNum = ui->threadNumEdit->text().toInt() - 1;
    for(int i=0; i<threadNum; i++)
    {
        PandaServerThread* newThread = new PandaServerThread(this);
        threadList.append(newThread);
        newThread->start();
    }
}

void PandaServer::CreateSocket(qintptr socketDescriptor)
{
    PandaSocket* newSocket = new PandaSocket(socketDescriptor);

    connect(this, &PandaServer::SignSendMsg, newSocket, &PandaSocket::SlotWriteData);
    connect(this, &PandaServer::SignDisconnected, newSocket, &PandaSocket::SlotDisconnected);

    newSocket->setSocketDescriptor(socketDescriptor);
    socketList.append(newSocket);

    emit SignAddInfo("SB", socketDescriptor);
}

void PandaServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new connection. descriptor: " << socketDescriptor;

    int index = GetMinLoadThreadIndex();
    if(index == -1)//无子线程
    {
        CreateSocket(socketDescriptor);
    }
    else
    {
        threadList[index]->CreateSocket(socketDescriptor);
    }

//    PandaSocket* socket = new PandaSocket(socketDescriptor);

//    connect(socket, &PandaSocket::SignSendMsg, this, &PandaServer::SlotSendClinetMsg);
//    connect(socket, &PandaSocket::SignDisconnected, this, &PandaServer::SlotDisconnected);

//    socket->setSocketDescriptor(socketDescriptor);
//    PandaSocketList.append(socket);

}

void PandaServer::SlotAddSocketInfo(QString userName, qintptr socketDescriptor)
{
    socketInformations.append({socketDescriptor, userName});
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

    emit SignSendMsg(msg);
    for(int i=0; i<socketList.count(); i++)
    {
        auto item = socketList.at(i);
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
    emit SignSendMsg(msg);

    ui->socketSended->append(msg);
    for(int i=0; i<socketList.count(); i++)
    {
        auto item = socketList.at(i);
        if(item->write(msg.toLatin1()))
        {
            continue;
        }
    }
}

void PandaServer::SlotDisconnected(qintptr descriptor)
{
    emit SignDisconnected(descriptor);
    for(int i=0; i<socketList.count(); i++)
    {
        auto item = socketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            socketList.removeAt(i);
            return;
        }
    }
    return;
}
